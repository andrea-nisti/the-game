FROM debian:stable-slim

# Install tar if it's not already installed
RUN apt-get update && apt-get install -y tar

# Create a directory for the application
RUN mkdir /app

# Copy the game tarball to the container
COPY *.tar game.tar
# Extract the tarball into the /app directory
RUN tar -xvf /game.tar -C /app

# Set the working directory
WORKDIR /app

# Expose port 8080
# EXPOSE 8080

# Run the game executable (replace 'game_executable' with the actual executable name)
# CMD ["bash"]
ENTRYPOINT [ "./game" ]