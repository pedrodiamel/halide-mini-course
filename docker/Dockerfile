# FROM ubuntu:18.04
FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu18.04

WORKDIR /workspace/

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    wget \
    curl \
    zip \
    unzip \
    byobu \
    vim \
    git \
    cmake \
    libjpeg-dev \
    libpng-dev \
    zlib1g-dev \
    libtiff-dev \
    python3-dev \
    python3-opencv \
    python3-pip \
    libopencv-core-dev \
    libopencv-highgui-dev \
    libopencv-imgproc-dev \
    libopencv-video-dev \
    libopencv-calib3d-dev \
    libopencv-features2d-dev \
    openjdk-8-jdk \
    libglfw3-dev

RUN curl -o ~/miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh && \
     chmod +x ~/miniconda.sh && \
     ~/miniconda.sh -b -p /opt/conda && \
     rm ~/miniconda.sh && \
     /opt/conda/bin/conda install -y python=$PYTHON_VERSION pip numpy pyyaml scipy ipython mkl mkl-include ninja cython typing && \
     /opt/conda/bin/conda clean -ya
ENV PATH /opt/conda/bin:$PATH

WORKDIR /opt

# install cmake 
RUN wget https://github.com/Kitware/CMake/releases/download/v3.21.1/cmake-3.21.1-linux-x86_64.sh
RUN mkdir cmake_linux; bash cmake-3.21.1-linux-x86_64.sh --prefix=cmake_linux --skip-license
# RUN apt-get install -y snapd
# RUN snap install cmake --classic

# build and install llvm 12
RUN git clone --depth 1 --branch llvmorg-12.0.0 https://github.com/llvm/llvm-project.git

RUN ./cmake_linux/bin/cmake -DCMAKE_BUILD_TYPE=Release \
          -DLLVM_ENABLE_PROJECTS="clang;lld;clang-tools-extra" \
          -DLLVM_TARGETS_TO_BUILD="X86;ARM;NVPTX;AArch64;Mips;Hexagon;WebAssembly" \
          -DLLVM_ENABLE_TERMINFO=OFF -DLLVM_ENABLE_ASSERTIONS=ON \
          -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_BUILD_32_BITS=OFF \
          -S llvm-project/llvm -B llvm-build

RUN ./cmake_linux/bin/cmake --build llvm-build -j=8
RUN ./cmake_linux/bin/cmake --install llvm-build --prefix llvm-install

ENV LLVM_ROOT /opt/llvm-install
ENV LLVM_CONFIG $LLVM_ROOT/bin/llvm-config

# build and install halide 
RUN git clone https://github.com/halide/Halide.git
RUN cd Halide; make -j8;

ENV HALIDE_PATH /opt/Halide
ENV PATH /opt/Halide/bin:$PATH
ENV LD_LIBRARY_PATH /opt/Halide/lib:$LD_LIBRARY_PATH
ENV DYLD_LIBRARY_PATH /opt/Halide/lib:$DYLD_LIBRARY_PATH
ENV HL_INCLUDE /opt/Halide/include
ENV HL_LIBRARY /opt/Halide/lib 

WORKDIR /workspace/course
