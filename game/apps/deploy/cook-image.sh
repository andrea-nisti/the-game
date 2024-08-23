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
        NAME=$OPTARG
        echo image name: $TAG
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
BAZEL_EXEC_ROOT=$(bazel info execution_root)
DEST_DIR=$SCRIPT_DIR
DOCKERFILE="Dockerfile"
# BASE_PATH = "../../bazel-bin/apps"

# Copy the tar file from Bazel exec root to the destination directory
cp "$(realpath -m "$SCRIPT_DIR/../../bazel-bin/apps/$TAR_FILE")" "$DEST_DIR/"

# Ensure the Dockerfile exists in the destination directory
if [ ! -f "$DOCKERFILE" ]; then
    echo "Dockerfile not found!"
    exit 1
fi

# Change to the destination directory
cd $DEST_DIR

# Build the Docker image
docker build -t game_image:$TAG .

# Print success message
echo "Docker image built successfully: game_image:$TAG"

echo "Removing temporary file: $DEST_DIR/$TAR_FILE"
rm -rf "$DEST_DIR/$TAR_FILE"
