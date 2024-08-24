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

EXPOSE 8888

# Define a parameter for the entrypoint executable
ARG EXECUTABLE_NAME
ENV APP ${EXECUTABLE_NAME}
CMD ./${APP}

# Run the game executable (replace 'EXECUTABLE_NAME' with the actual executable name)
# ENTRYPOINT [ echo ./$APP ]
