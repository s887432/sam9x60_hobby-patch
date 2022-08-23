# SAM9x60 Hobby board patch based on Linux4sam 2022.07
Here is the build process.<br>
All are done in Ubuntu<br>

$ git clone https://github.com/linux4sam/buildroot-at91.git -b sam9x60-curiosity-2022.07<br>
$ git clone https://github.com/linux4sam/buildroot-external-microchip.git -b sam9x60-curiosity-2022.07<br>

Create patch folder in buildroot-external-microchip<br>
$ mkdir -p buildroot-external-microchip/patches/linux<br>

Copy patch file to buildroot-external-microchip/patches/linux<br>

$ cd buildroot-at91<br>
$ BR2_EXTERNAL=../buildroot-external-microchip make sam9x60_curiosity_graphics_defconfig<br>
$ make<br>

Have fun!<br>


Patrick Lin @ Taipei, 2022/Aug<br>
