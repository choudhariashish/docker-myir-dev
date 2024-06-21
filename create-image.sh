#!/bin/bash

sudo apt-get install docker.io

if [ ! -f downloads/04-Source.tar.bz2 ]; then
    wget https://d.myirtech.com/MYS-8MMX/L5.4.3/04-Source.tar.bz2
    tar -xf 04-Source.tar.bz2
    mkdir downloads
    mv 04-Source.tar.bz2 downloads/
fi


if [ ! -f share/04-Source/mys-8mmx-yocto.tar.gz ]; then
    mkdir share
    mv 04-Source/ share/
    cd share/04-Source/
    tar xf mys-8mmx-yocto.tar.gz
    cd imx-yocto-bsp/

    echo "#!/bin/bash" >> build.sh
    echo "DISTRO=fsl-imx-xwayland MACHINE=mys-8mmx source sources/meta-myir/tools/myir-setup-release.sh -b build-xwayland" >> build.sh
    echo "bitbake core-image-minimal" >> build.sh
    chmod +x build.sh

    cd ../../../
    chmod -R 777 share/
fi

sudo docker build -t myir-dev .
