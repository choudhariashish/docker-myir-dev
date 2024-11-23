cp ../share/04-Source/imx-yocto-bsp/build-xwayland/tmp/deploy/images/mys-8mmx/imx-boot .
cp ../share/04-Source/imx-yocto-bsp/build-xwayland/tmp/deploy/images/mys-8mmx/fitimage.fit .
cp fitimage.fit /tftpboot

printf "\n\nDo the following on uboot cli...\n\n"
printf "tftpboot 0x60000000 fitimage.fit\n"
printf "bootm 0x60000000\n"
