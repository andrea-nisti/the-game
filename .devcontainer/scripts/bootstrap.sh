#!/usr/bin/env bash
set -e

BIN_DIR=${HOME}/.local/bin
cd "${BIN_DIR}"

# Download starpls if missing
if [[ ! -f starpls ]]; then
    wget -O starpls.tar.gz https://github.com/withered-magic/starpls/releases/download/v0.1.21/starpls-linux-amd64.tar.gz
    tar -xf starpls.tar.gz && rm -rf starpls.tar.gz
    chmod +x starpls
fi

# Ensure brew is initialized
eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"
brew update
brew bundle --global install
