#!/bin/bash
source ../../scripts/pluto.sh
cp -r ../../meta-myir sources/
cp -r ../../meta-pluto sources/
DISTRO=fsl-imx-xwayland MACHINE=mys-8mmx source sources/meta-myir/tools/myir-setup-release.sh -b build-xwayland
ls -l
bitbake-layers add-layer ../sources/meta-pluto/
bitbake pluto-image-core
bitbake pluto-image-core -c fitimage -f
