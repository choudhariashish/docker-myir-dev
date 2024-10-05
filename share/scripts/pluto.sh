#!/bin/bash

pluto_extract_rootfs () {
  rm -rf rootfs
  mkdir rootfs
  cp build-xwayland/tmp/deploy/images/mys-8mmx/pluto-image-core-mys-8mmx.tar.bz2 rootfs/
  cd rootfs
  tar -xf pluto-image-core-mys-8mmx.tar.bz2
}


pluto_read_sdcard_partition () {
  sudo sfdisk -d $1
}


pluto_write_sdcard_partition () {
  sudo sfdisk $1 < $2 
}


pluto_boot_scr_create () {

    scrname=boot.script
    if [ -e "$scrname" ]; then
        mkimage -c none -A arm -T script -d boot.script boot.scr
    else 
        echo "File $scrname does not exist in $(pwd)"
    fi 
}


pluto_help () {
    echo ""
    echo "pluto=>"
    echo "pluto_extract_rootfs         - Will extract a rootfs to inspect the filesystem. Note: Have to be in the yocto root"
    echo "pluto_read_sdcard_partition  - Will show partition table on the drive. ex=> pluto_read_sdcard_partition /dev/mmcblk1 > partition-table"
    echo "pluto_write_sdcard_partition - Will write the drive with partition table. ex=> pluto_write_sdcard_partition /dev/mmcblk1 partition-table"
    echo "pluto_boot_scr_create        - Will create a boot.scr script. Note: Have to be in the scripts directory"
    echo ""
    echo "devtool=>"
    echo "    devtool modify <recipename>                       - Will clone the correct directory in the workspace."
    echo "    devtool build <recipename>                        - Will build the cloned directory."
    echo "    devtool update-recipe -a <layerpath> <recipename> - Will append the changes/patches to the recipe in specified layerpath."
    echo "    devtool reset <recipename>                        - Will cleanup the workspace for the recipe."
}
