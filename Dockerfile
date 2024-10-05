FROM ubuntu:18.04

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt update -y
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y apt-utils
RUN apt-get install -y python3
RUN apt-get install -y python3-distutils
RUN apt-get install -y python3-distutils-extra
RUN apt-get install -y lsb-core
RUN apt-get install -y wget
RUN apt-get install -y chrpath
RUN apt-get install -y texinfo
RUN apt-get install -y python
RUN apt-get install -y vim
RUN apt-get install -y diffstat
RUN apt-get install -y gawk
RUN apt-get install -y git


RUN locale-gen en_US en_US.UTF-8
RUN dpkg-reconfigure --frontend=noninteractive locales && update-locale LANG=en_US.UTF-8
RUN echo 'export LC_ALL=en_US.UTF-8' >> /etc/environment


RUN useradd -ms /bin/bash $USERNAME
USER $USERNAME
WORKDIR /home/$USERNAME

RUN echo 'export LC_ALL=en_US.UTF-8' >> /home/$USERNAME/.bashrc
