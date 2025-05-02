FROM mcr.microsoft.com/devcontainers/cpp:ubuntu-22.04

# Expose port 8888
EXPOSE 8888

# Install misc deps
RUN mkdir /home/vscode/scripts
COPY scripts/install-deps.sh /home/vscode/scripts
RUN chmod +x /home/vscode/scripts/install-deps.sh
