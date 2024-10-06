# docker-myir-dev

## One-time docker setup (Host).
```
sudo apt-get install docker.io
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker
```

## One-time tftp server setup (Host).
```
sudo apt-get update && sudo apt-get install xinetd tftpd tftp
```

edit /etc/xinetd.d/tftp to,
```
service tftp
{
     protocol = udp
     port = 69
     socket_type = dgram
     wait = yes
     user = nobody
     server = /usr/sbin/in.tftpd
     server_args = /tftpboot
     disable = no

}

sudo mkdir /tftpboot/
sudo chmod -R 777 /tftpboot/
sudo chown -R nobody /tftpboot/
sudo service xinetd restart

sudo ip addr add 192.168.1.30/24 dev eno1
```

## This script will create a docker image (Host).
```
./create-image.sh
```

## This script will start the image (Host).
```
./start-image.sh
```

## Image/OS development workflow.
### Build Image (Docker).
```
cd /share/04-Source/imx-yocto-bsp && ./core.sh
```

### Artifacts (Host).
```
cd out/ && ./fetch-images.sh
```

### Prepare sdcard (Host).
```
coming soom...
```

### Flash uboot/bootloader on a sdcard (Host). This can also be done on a running image onboard.
```
sudo dd if=imx-boot of=/dev/<your mmc> bs=1k seek=33
```

### Do the following on uboot/bootloader cli (Board)"
```
tftpboot 0x10000000 fitimage.fit
bootm 0x10000000
```

## Application development workflow.
```
coming soom...
```