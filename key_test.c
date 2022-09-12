#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

typedef enum __LEDS__
{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE, 
	LED_NONE
} LED;

typedef enum __DELAYS__
{
	DELAY_100MS = 100,
	DELAY_300MS = 300,
	DELAY_500MS = 500,
	DELAY_1SEC = 1000
}DELAY;

static char leds_handle[][50] = {
	"/sys/class/leds/red/brightness",
	"/sys/class/leds/green/brightness",
	"/sys/class/leds/blue/brightness"
};

int leds[3];

LED gCurLed = LED_NONE;
int gDelay = 0;
int changed = 0;

pthread_mutex_t lock;

void led_on(LED led)
{
	write(leds[led], "1", 1);
}

void led_off(LED led)
{
	write(leds[led], "0", 1);
}

void *led_proc()
{
	int keepGoing = 1;
	LED led;
	int delay;
	
	led = gCurLed;
	delay = gDelay;
	
	do
	{
		if( gCurLed != LED_NONE )
		{
			led_on(led);
			usleep(gDelay*1000);
			led_off(led);
			usleep(gDelay*1000);
			
			pthread_mutex_lock(&lock);
			if( changed ) {
				changed = 0;
				led = gCurLed;
				delay = gDelay;
			}
			pthread_mutex_unlock(&lock);
		}
		
	}while(keepGoing);
}

int led_init(void)
{
	int fd;
	
	if( (fd = open("/sys/class/leds/blue/trigger", O_WRONLY)) <= 0 )
	{
		printf("open blue led trigger error\r\n");
		return -1;
	}

	write(fd, "none", 1);
	
	close(fd);
	
	if( (fd = open("/sys/class/leds/red/trigger", O_WRONLY)) <= 0 )
	{
		printf("open red led trigger error\r\n");
		return -1;
	}

	write(fd, "none", 1);
	
	close(fd);
	
	if( (fd = open("/sys/class/leds/green/trigger", O_WRONLY)) <= 0 )
	{
		printf("open green led trigger error\r\n");
		return -1;
	}

	write(fd, "none", 1);
	
	close(fd);
	

	for(int i=0; i<3; i++)
	{
		if( (leds[i] = open(leds_handle[i], O_WRONLY)) <= 0 )
		{
			printf("open led %d brightness error\r\n", i);
			return -1;
		}
		write(leds[i], 0, 1);
	}
	
	return 0;
}

int find_key_event(char *device_name)
{
	int fd;
	int i;
	char name[64];
	char buf[256] = {0};
	
	for(i=0; i<32; i++)
	{
		sprintf(name, "/dev/input/event%d", i);
		if( (fd = open(name, O_RDONLY, 0)) >= 0 )
		{
			ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
			if( strcmp(buf, "gpio-keys") == 0 )
			{
				strcpy(device_name, name);
				close(fd);
				return 0;
			}
			
			close(fd);
		}
	}
	
	return -1;
}

int main(int argc, char **argv)
{
	int keys_fd;
	struct input_event t;
	pthread_t tLed;
	int ret_val_t1;
	LED led;
	int delay;
	char device_name[256];
	
	if( find_key_event(device_name) < 0 )
	{
		printf("Can't find input device\r\n");
		return -1;
	}

	if( (keys_fd = open(device_name, O_RDONLY)) <= 0 )
	{
		printf("open event error\r\n");
		return -1;
	}
	
	led_init();
	led = gCurLed = LED_RED;
	delay = gDelay = DELAY_100MS;
	ret_val_t1 = pthread_create( &tLed, NULL, led_proc, NULL);
	if(ret_val_t1)
	{
		fprintf(stderr,"Error - pthread_create() return value: %d\n",ret_val_t1);
		exit(EXIT_FAILURE);
	}
	
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
	while(1)
	{
		if( read(keys_fd, &t, sizeof(t)) == sizeof(t))
		{
			if( t.type == EV_KEY )
			{
				if( t.value == 0 )
				{
					// key release
					switch( t.code )
					{
						case KEY_PROG1:
							if( led == LED_RED )
								led = LED_GREEN;
							else if( led == LED_GREEN )
								led = LED_BLUE;
							else if( led == LED_BLUE )
								led = LED_RED;
								
							pthread_mutex_lock(&lock);
								changed = 1;
								gCurLed= led;
							pthread_mutex_unlock(&lock);
							break;
							
						case KEY_PROG2:
							if( delay == DELAY_100MS )
								delay = DELAY_300MS;
							else if( delay == DELAY_300MS )
								delay = DELAY_500MS;
							else if( delay == DELAY_500MS )
								delay = DELAY_1SEC;
							else if( delay == DELAY_1SEC )
								delay = DELAY_100MS;
								
							pthread_mutex_lock(&lock);
								changed = 1;
								gDelay= delay;
							pthread_mutex_unlock(&lock);
							break;
					}
				}
			}
		}
	}

	close(keys_fd);
	return 0;
}

// end of file

