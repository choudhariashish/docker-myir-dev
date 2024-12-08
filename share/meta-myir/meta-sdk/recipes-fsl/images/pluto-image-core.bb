# Copyright (C) 2015 Freescale Semiconductor
# Copyright 2017-2019 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

#DESCRIPTION = "NXP Image to validate i.MX machines. \
#This image contains everything used to test i.MX machines including GUI, \
#demos and lots of applications. This creates a very large image, not \
#suitable for production."
#LICENSE = "MIT"

SUMMARY = "A minimum Pluto Image."
DESCRIPTION = "A minimum Pluto Image."
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""


require recipes-fsl/images/imx-image-multimedia.bb
CONFLICT_DISTRO_FEATURES = "directfb"

inherit logging
inherit core-image

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

create_fitimage() {
    ARTIFACTS_DIR=${DEPLOY_DIR}/images/mys-8mmx/

cat << EOF > ${ARTIFACTS_DIR}/pluto-image.its
/dts-v1/;
/ {
    description = "Nitrogen8M i.MX8M FIT Image";
    #address-cells = <1>;
    images {
        kernel {
            description = "Kernel";
            data = /incbin/("Image");
            type = "kernel";
            arch = "arm64";
            os = "linux";
            compression = "none";
            load = <0x40480000>;
            entry = <0x40480000>;
            hash {
                algo = "sha1";
            };
        };
        fdt {
            description = "DTB";
            data = /incbin/("mys-imx8mm-lt8912-hontron-7-v20-mys-8mmx.dtb");
            type = "flat_dt";
            arch = "arm64";
            compression = "none";
            load = <0x48000000>;
            entry = <0x48000000>;
            hash {
                algo = "sha1";
            };
        };
        initrd {
            description = "Initrd";
            data = /incbin/("pluto-image-core-mys-8mmx.cpio.gz");
            type = "ramdisk";
            arch = "arm64";
            os = "linux";
            compression = "none";
            load = <0x00000000>;
            entry = <0x00000000>;
            hash {
                algo = "sha1";
            };
        };
    };
    configurations {
        default = "standard";
        standard {
            description = "Standard Boot";
            kernel = "kernel";
            fdt = "fdt";
            ramdisk = "initrd";
            hash {
                algo = "sha1";
            };
        };
    };
};
EOF
    cd ${ARTIFACTS_DIR} && mkimage -f pluto-image.its fitimage.fit
}


# Shell function example.
do_plutosh_rootfs_postprocess_command() {
    bbwarn "do_plutosh_rootfs_postprocess_command"
    create_fitimage
}


# Python function example.
fakeroot python do_plutopy_rootfs_postprocess_command () {
    bb.warn("do_plutopy_rootfs_postprocess_command")
}


# https://blog.linumiz.com/archives/16540
ROOTFS_POSTPROCESS_COMMAND += "do_plutosh_rootfs_postprocess_command; do_plutopy_rootfs_postprocess_command;"
