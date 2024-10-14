FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

# Install misc deps
COPY ./install-deps.sh /tmp/

RUN chmod +x /tmp/install-deps.sh && /tmp/install-deps.sh

