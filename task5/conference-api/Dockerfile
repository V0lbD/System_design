FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

WORKDIR /app

COPY . .

RUN make build-debug

EXPOSE 8080

CMD ["./build-debug/conference_api", "--config", "./configs/static_config.yaml", "--config_vars", "./configs/config_vars.yaml"]