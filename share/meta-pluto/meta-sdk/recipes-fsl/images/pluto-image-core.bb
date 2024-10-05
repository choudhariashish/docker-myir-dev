# Copyright (C) 2015 Freescale Semiconductor
# Copyright 2017-2019 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "NXP Image to validate i.MX machines. \
This image contains everything used to test i.MX machines including GUI, \
demos and lots of applications. This creates a very large image, not \
suitable for production."
LICENSE = "MIT"

inherit core-image

### WARNING: This image is NOT suitable for production use and is intended
###          to provide a way for users to reproduce the image used during
###          the validation process of i.MX BSP releases.

IMAGE_FSTYPES += " cpio.gz"

## Select Image Featurest
IMAGE_FEATURES += " \
    splash \
    nfs-server \
    ssh-server-dropbear \
    hwcodecs \
"

CORE_IMAGE_EXTRA_INSTALL += " \
    ntp \
    tcpdump \
    ${@bb.utils.contains('MACHINENAME', 'myd-imx8mm', 'start-service', '', d)} \
    ${@bb.utils.contains('MACHINENAME', 'myd-imx8mm', 'staticip-network', '', d)} \
    autorun-script \
    pluto \
"
