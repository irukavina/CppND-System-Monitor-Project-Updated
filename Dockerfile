FROM gcc:9.2

RUN apt-get update && \
    apt-get install -y --no-install-recommends clang-format \
                                               gdb

# Cmake installation inspired by https://hub.docker.com/r/rikorose/gcc-cmake/dockerfile
RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.5/cmake-3.15.5-Linux-]x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh
ENV PATH="/usr/bin/cmake/bin:${PATH}"
