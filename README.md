# SAM9x60 Hobby board patch based on Linux4sam 2022.10
Here is the build process.<br>
All are done in Ubuntu<br>

$ git clone https://github.com/linux4sam/buildroot-at91.git -b linux4sam-2022.10<br>
$ git clone https://github.com/linux4microchip/buildroot-external-microchip.git -b linux4microchip-2022.10<br>

Apply patch for buildroot<br>
$ cd buildroot-external-microchip<br>
$ git apply 0000_sam9x60_hobby_buildroot_external.patch<br>

Build buildroot for hobby board
$ cd ../buildroot-at91<br>
$ BR2_EXTERNAL=../buildroot-external-microchip make sam9x60_hobby_graphics_defconfig<br>
$ make<br>

Have fun!<br>


Patrick Lin @ Taipei, 2023/Mar<br>
