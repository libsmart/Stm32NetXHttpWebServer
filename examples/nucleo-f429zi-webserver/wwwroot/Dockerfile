FROM php:8.3-alpine

LABEL authors="roland.rusch@easy-smart.ch"

# Install mitmproxy and Supervisor
RUN apk update && apk add --no-cache \
    python3 \
    py3-pip \
    supervisor \
    bash \
    curl \
    mitmproxy \
    && rm -rf /var/cache/apk/*

COPY ./supervisord.conf /etc/supervisor/supervisord.conf
#RUN mkdir -pv /opt/supervisord && mkdir -pv /opt/mitmproxy

RUN cp "$PHP_INI_DIR/php.ini-production" "$PHP_INI_DIR/php.ini"

WORKDIR /opt/supervisord
EXPOSE 18080 18081 18000

#ENTRYPOINT ["ash"]
ENTRYPOINT ["/usr/bin/supervisord", "-c", "/etc/supervisor/supervisord.conf"]
