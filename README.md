# docker-myir-dev

## One-time docker setup.
```
sudo apt-get install docker.io
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker
```

## One-time tftp server setup.
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

## This script will create a docker image.
```
./create-image.sh
```

## This script will start the image.
```
./start-image.sh
```

## Build Image.
```
cd /share/04-Source/imx-yocto-bsp && ./core.sh
```

## Artifacts.
```
cd out/ && ls
```

## Flash uboot.
```
sudo dd if=imx-boot of=/dev/mmcblk0 bs=1k seek=33
```
