FROM ubuntu
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && \
    apt-get install -y gcc gfortran libbz2-dev fort77 bzip2 xorg-dev liblzma-dev libblas-dev gfortran gcc-multilib gobjc++ aptitude libreadline-dev\
     libjpeg-dev libssl-dev libcurl4-openssl-dev libxml2-dev libpng-dev build-essential git libpcre2-dev littler \
      xvfb xauth xfonts-base texlive-latex-base texlive-fonts-extra pandoc && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
# install dyntrace
RUN git clone https://github.com/PRL-PRG/R-dyntrace.git && cd R-dyntrace && bash -c "git checkout r-4.0.2" && \
    ./build --with-readline=no --with-x=no
# Install injectr
RUN cd /home && git clone https://github.com/nsaripalli/injectr.git && cd injectr && git checkout patch-1 && \
    xvfb-run --auto-servernum --server-num=1 make R="/R-dyntrace/bin/R" clean build install
WORKDIR /home/instrumentr
ADD . /home/instrumentr
# Install instrumentr
RUN cd /home/instrumentr && xvfb-run --auto-servernum --server-num=1 make R="/R-dyntrace/bin/R" clean document build check install
CMD /bin/bash
ENTRYPOINT /R-dyntrace/bin/R --no-save