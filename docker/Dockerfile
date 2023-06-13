#
# Ubuntu Dockerfile
#
# https://github.com/dockerfile/ubuntu
#

# Pull base image.
FROM  opencvcourses/opencv-docker:latest
ARG OPENCV_VERSION=4.2.0 

# Install.
RUN \
  sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
  apt-get update && \
  apt-get -y upgrade && \ 
  apt-get install -y tzdata && \
  apt-get install -y build-essential cmake && \
  apt-get install -y software-properties-common && \
  apt-get install -y curl man unzip vim wget && \
  apt-get install -y qemu qemu-system-arm gcc-arm-none-eabi && \ 
  rm -rf /var/lib/apt/lists/*

# Set environment variables.
ENV HOME /root
ENV TZ=America/Buenos_Aires
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Define working directory.
WORKDIR /root
# Define default command.
CMD ["bash"]