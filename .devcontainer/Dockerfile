FROM mcr.microsoft.com/devcontainers/cpp

# Copy bootstrap script and brew dependencies
COPY scripts/bootstrap.sh /home/vscode/scripts/bootstrap.sh
COPY scripts/Brewfile /home/vscode/.Brewfile
RUN  chmod +x /home/vscode/scripts/bootstrap.sh /home/vscode/.Brewfile
RUN  chown -R vscode:vscode /home/vscode/scripts/

# Switch to vscode user
WORKDIR /home/vscode
USER vscode
RUN mkdir -p .local/bin

# Install Homebrew in non-interactive mode
RUN NONINTERACTIVE=1 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" && \
    echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> ~/.bashrc && sudo chown -R vscode:vscode /home/linuxbrew/.linuxbrew

# Add brew and llvm to PATH for Dockerfile RUN steps
ENV PATH="/home/linuxbrew/.linuxbrew/bin: \
    /home/linuxbrew/.linuxbrew/sbin: \
    /home/linuxbrew/.linuxbrew/opt/llvm/bin: \
    /home/linuxbrew/.linuxbrew/opt/binutils/bin:${PATH}"

# Run bootstrap once (downloads Bazel LSP, installs brew deps)
RUN scripts/bootstrap.sh /workspaces/the-game/game
