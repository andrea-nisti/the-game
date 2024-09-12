#!/usr/bin/env bash
#
set -e

BIN_DIR=$HOME/.local/bin

if [ ! -d "$BIN_DIR" ]; then
    mkdir -p $BIN_DIR
fi

# Download bazel buildifier
wget -O $BIN_DIR/buildifier https://github.com/bazelbuild/buildtools/releases/download/v7.3.1/buildifier-linux-amd64
chmod +x $BIN_DIR/buildifier