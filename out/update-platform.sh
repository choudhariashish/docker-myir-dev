#!/bin/bash

ssh-keygen -f "/home/$USER/.ssh/known_hosts" -R "192.168.1.5"

while getopts 'mfark:h' opt; do
  case "$opt" in
    m)
      echo "Updating mcu application..."
      scp m4.bin root@192.168.1.5:/run/media/mmcblk1p1/
      ssh root@192.168.1.5 /bin/sync
      ;;

    f)
      echo "Updating fitimage..."
      scp fitimage.fit root@192.168.1.5:/run/media/mmcblk1p1/p1/
      ssh root@192.168.1.5 /bin/sync
      ;;

    a)
      echo "Updating applications..."
      scp boot.scr root@192.168.1.5:/run/media/mmcblk1p1/
      ssh root@192.168.1.5 /bin/sync
      ;;

    r)
      echo "Rebooting platform..."
      ssh root@192.168.1.5 /sbin/reboot
      ;;


    ?|h)
      echo "Usage: $(basename $0) [-m] [-f] [-a] [-r]"
      echo "m-mcu application, f-fitimage, a-applications, r-reboot platform"
      exit 1
      ;;
  esac
done
shift "$(($OPTIND -1))"
