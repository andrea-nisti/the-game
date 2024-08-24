#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Usage function
usage() {
    echo "Usage: $0 -n <name> -t <tag> -f <tar_file>"
    echo "  -n <tag>        Docker image name"
    echo "  -t <tag>        Docker image tag"
    echo "  -f <tar_file>   Name of the tar file in the Bazel exec root (e.g., bazel-bin/apps/game.tar)"
    echo "  -h print help"
    exit 1
}

# Parse command-line arguments
while getopts "n:t:f:h" opt; do
    case $opt in
    n)
        IMAGE_NAME=$OPTARG
        echo image name: $IMAGE_NAME
        ;;
    t)
        TAG=$OPTARG
        echo image tag: $TAG
        ;;
    f)
        TAR_FILE=$OPTARG
        echo file: $TAR_FILE
        ;;
    h)
        usage
        ;;
    esac
done

# Check if both arguments are provided
if [ -z "$TAG" ] || [ -z "$TAR_FILE" ] || [ -z "$NAME" ]; then
    usage
fi

# Variables
# Get the directory containing the script

SCRIPT_DIR=$(dirname $(realpath "$0"))
DEST_DIR=$SCRIPT_DIR
DOCKERFILE="Dockerfile"

for f in "$DEST_DIR"/*.tar; do
    if [ -f "$f" ]; then
        echo "Removing temporary file: $f"
        rm -rf "$f"
    fi
done

# Copy the tar file from Bazel exec root to the destination directory
cp "$(realpath -m "$SCRIPT_DIR/../../bazel-bin/apps/$TAR_FILE")" "$DEST_DIR/"

# Ensure the Dockerfile exists in the destination directory
if [ ! -f "$DOCKERFILE" ]; then
    echo "Dockerfile not found!"
    exit 1
fi

# Strip "_tar" from the tar file name to get the executable name (e.g., "game_tar" -> "game")
# This is the name of the file that will be the entrypoint in the Docker image
EXECUTABLE_NAME=$(basename $TAR_FILE .tar)
EXECUTABLE_NAME="${EXECUTABLE_NAME%_tar}"
echo executable name: $EXECUTABLE_NAME

# Change to the destination directory
cd $DEST_DIR

# Build the Docker image
docker build -t $IMAGE_NAME:$TAG --build-arg "EXECUTABLE_NAME=$EXECUTABLE_NAME" .

# Print success message
echo "Docker image built successfully: $IMAGE_NAME:$TAG"

echo "Removing temporary file: $DEST_DIR/$TAR_FILE"
for f in "$DEST_DIR"/*.tar; do
    if [ -f "$f" ]; then
        rm -rf "$f"
    fi
done
