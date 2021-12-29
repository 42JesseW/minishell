FROM debian:buster

ARG APP_DIR=/app

ADD includes    ${APP_DIR}/includes
ADD libft       ${APP_DIR}/libft
ADD srcs        ${APP_DIR}/srcs
COPY ["Makefile", "CMakeLists.txt", "${APP_DIR}/"]

RUN apt-get update \
    && apt-get install -y \
        valgrind \
        make \
        gcc \
        libreadline-dev
