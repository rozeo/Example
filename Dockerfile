FROM ubuntu:latest

RUN apt update && \
    apt install -y git curl vim

RUN git clone https://github.com/CHH/phpenv.git

RUN ./phpenv/bin/phpenv-install.sh

RUN echo 'export PATH="/root/.phpenv/bin:$PATH"; eval "$(phpenv init -)";' >> ~/.bashrc

ENV PATH /root/.phpenv/bin:$PATH

RUN git clone https://github.com/CHH/php-build.git ~/.phpenv/plugins/php-build

# for php build
RUN apt install -y \
    gcc g++ \
    libssl-dev \
    libreadline-dev \
    libxml2-dev \
    libcurl4-openssl-dev \
    libpng-dev libjpeg-dev \
    libmcrypt-dev \
    libtidy-dev \
    autoconf \
    bison \
    libxslt-dev \
    libkrb5-dev \
    libsqlite3-dev \
    libzip-dev \
    libbz2-dev \
    libonig-dev

RUN phpenv install 7.4.1

