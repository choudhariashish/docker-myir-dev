#!/bin/bash

if [ ! -f downloads/04-Source.tar.bz2 ]; then
    wget https://d.myirtech.com/MYS-8MMX/L5.4.3/04-Source.tar.bz2
    tar -xf 04-Source.tar.bz2
    mkdir downloads
    mv 04-Source.tar.bz2 downloads/
fi


if [ ! -f share/04-Source/mys-8mmx-yocto.tar.gz ]; then
    mv 04-Source/ share/

    cd share/04-Source/
    tar xf mys-8mmx-yocto.tar.gz
    cp -r ../meta-pluto imx-yocto-bsp/sources/meta-pluto
    cd imx-yocto-bsp/

    echo "#!/bin/bash" >> core.sh
    echo "cp -r ../../meta-pluto sources/" >> core.sh
    echo "DISTRO=fsl-imx-xwayland MACHINE=mys-8mmx source sources/meta-pluto/tools/pluto-setup-release.sh -b build-xwayland" >> core.sh
    echo "bitbake pluto-image-core" >> core.sh
    echo "source ../../scripts/pluto.sh" >> core.sh
    chmod +x core.sh

    cd ../../../

    # Patch the sources.
    ./apply-patch.sh patches/pluto-patch-1.diff share/04-Source/imx-yocto-bsp/sources/
    ./apply-patch.sh patches/pluto-patch-2.diff share/04-Source/imx-yocto-bsp/sources/
    chmod -R 777 share/
fi

docker build -t myir-dev .
#scp -r choudhariashish@192.168.86.25:/var/services/homes/choudhariashish/dev/MYS-8MMX-V2-SDK/sdk/downloads .
