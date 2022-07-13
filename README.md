# sam9x60_hobby-patch
1. download sam9x60 Linux4sam buildroot sd card image<br>
https://files.linux4sam.org/pub/demo/linux4sam-2022.04/linux4sam-buildroot-sam9x60ek-graphics-2022.04.img.bz2<br>

2. flash image to micro SD card<br>

3. download patch
$ git clone https://github.com/s887432/sam9x60_hobby-patch.git<br>

4. download and build at91bootstrap
$ git clone https://github.com/linux4sam/at91bootstrap.git<br>
$ cd at91bootstrap<br>
$ make sam9x60_curiositysd_uboot_defconfig <br>
$ make<br>
$ cd ..

output binary is located at at91bootstrap/build/binaries/boot.bin<br>

5. download and build u-boot<br>
$ git clone https://github.com/linux4sam/u-boot-at91.git<br>
$ cd u-boot-at91<br>
$ make sam9x60_curiosity_mmc_defconfig<br>
$ make<br>
$ cd ..<br>

output binary is located at u-boot-at91/u-boot.bin<br>

6. download and build kernel<br>
$ git clone https://github.com/linux4microchip/kernel.git<br>
$ cd linux<br>

apply patch<br>
$ git apply ../sam9x60_hobby-patch/0001_linux.patch<br>
$ make at91_dt_defconfig<br>
$ make<br>
$ export KERNEL_DIR=$PWD<br>
$ cd ..

7 download and build dt-overlay<br>
$ git clone https://github.com/linux4microchip/dt-overlay-at91.git<br>
$ cd dt-overlay-at91<br>
$ make sam9x60_curiosity_dtbos<br>
$ make sam9x60_curiosity.itb<br>
$ cd ..

update built image to sd card<br>
boot.bin<br>
u-boot.bin<br>
sam9x60_curiosity.itb<br>

the default u-boot argument is not accurate<br>
need to update in u-boot concole<br>
U-Boot> setenv board_name 'sam9x60_curiosity'<br>
U-Boot> setenv bootcmd_boot 'fatload mmc 0:1 ${loadaddr} ${board_name}.itb; bootm ${loadaddr}#kernel_dtb#pda5'<br>
U-Boot> saveenv<br>

reboot and have fun<br>

Patrick Lin @ Taipei, 2022/July<br>
