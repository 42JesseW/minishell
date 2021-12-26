FROM alpine:3.15.0

ARG APP_DIR=/app

ADD includes    ${APP_DIR}/includes
ADD libft       ${APP_DIR}/libft
ADD srcs        ${APP_DIR}/srcs
COPY ["Makefile", "CMakeLists.txt", "${APP_DIR}/"]

RUN apk update \
    && apk add --no-cache \
        valgrind
