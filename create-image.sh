#!/bin/bash

if [ ! -f sdk-downloads/04-Source.tar.bz2 ]; then
    wget https://d.myirtech.com/MYS-8MMX/L5.4.3/04-Source.tar.bz2
    mkdir sdk-downloads
    mv 04-Source.tar.bz2 sdk-downloads/
fi


if [ ! -f share/04-Source/mys-8mmx-yocto.tar.gz ]; then

    echo "Extracting SDKs..."
    tar -jxf sdk-downloads/04-Source.tar.bz2 -C share/

    cd share/04-Source/
    tar xf mys-8mmx-yocto.tar.gz
    tar -xvzf m4.tar.gz

    echo "Setup M4 build tools/SDK..."
    cd m4/
    mkdir toolchains
    tar -jxf gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 -C toolchains/
    mkdir SDK_2.8.0_EVK-MIMX8MM
    tar -zxf SDK_2.8.0_EVK-MIMX8MM.tar.gz -C SDK_2.8.0_EVK-MIMX8MM/
    cp ../../scripts/setup_m4_env.sh .
    cd ../

    echo "Setup Linux build tools/SDK..."
    cp -r ../meta-pluto imx-yocto-bsp/sources/meta-pluto
    cd imx-yocto-bsp/

    echo "Prepare build scipt for yocto..."
    echo "#!/bin/bash" >> core.sh
    echo "cp -r ../../meta-pluto sources/" >> core.sh
    echo "DISTRO=fsl-imx-xwayland MACHINE=mys-8mmx source sources/meta-pluto/tools/pluto-setup-release.sh -b build-xwayland" >> core.sh
    echo "bitbake pluto-image-core" >> core.sh
    echo "source ../../scripts/pluto.sh" >> core.sh
    chmod +x core.sh

    cd ../../../

    echo "Patch the sources..."
    ./apply-patch.sh patches/pluto-patch-1.diff share/04-Source/imx-yocto-bsp/sources/
    ./apply-patch.sh patches/pluto-patch-2.diff share/04-Source/imx-yocto-bsp/sources/
    chmod -R 777 share/
fi

docker build -t myir-dev .
#scp -r choudhariashish@192.168.86.25:/var/services/homes/choudhariashish/dev/MYS-8MMX-V2-SDK/sdk/downloads .
