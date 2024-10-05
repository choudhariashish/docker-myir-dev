cp ../share/04-Source/imx-yocto-bsp/build-xwayland/tmp/deploy/images/mys-8mmx/mys-imx8mm-lt8912-hontron-7-v20-mys-8mmx.dtb system.dtb
cp ../share/04-Source/imx-yocto-bsp/build-xwayland/tmp/deploy/images/mys-8mmx/Image .
cp ../share/04-Source/imx-yocto-bsp/build-xwayland/tmp/deploy/images/mys-8mmx/pluto-image-core-mys-8mmx.cpio.gz rootfs.cpio.gz

cp ../share/scripts/pluto-image.its .

mkimage -f pluto-image.its fitimage.fit
cp fitimage.fit /tftpboot

rm *.dtb Image rootfs.cpio.gz *.its

printf "\n\nDo the following on uboot cli...\n\n"
printf "tftpboot 0x10000000 fitimage.fit\n"
printf "bootm 0x10000000\n"
