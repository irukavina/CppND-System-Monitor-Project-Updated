FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends  apt-transport-https \
                                                ca-certificates \
                                                gnupg \
                                                software-properties-common \
                                                wget


RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
RUN apt-get update
RUN apt-get install -y --no-install-recommends  cmake

RUN apt-get update && \
    apt-get install -y --no-install-recommends  clang-format \
                                                build-essential \
                                                gdb \
                                                git

RUN apt-get install -y --no-install-recommends  libncurses5-dev \
                                                libncursesw5-dev

WORKDIR /opt/exercises
