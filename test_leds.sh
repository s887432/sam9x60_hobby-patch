#!/bin/bash

LED_R=/sys/class/leds/red
LED_G=/sys/class/leds/green
LED_B=/sys/class/leds/blue

function flick_test()
{
        echo 1 > $1/brightness
        sleep 1
        echo 0 > $1/brightness
        sleep 1
}

echo "Start LED test."
echo "Will turn off LED Blue first"

echo none > $LED_B/trigger

for ((i=1;i<=10;i++))
do
        echo "Testing LED Red"
        flick_test $LED_R
        echo "Testing LED Green"
        flick_test $LED_G
        echo "Testing LED Blue"
        flick_test $LED_B
done

echo "LED Testinf is done"
