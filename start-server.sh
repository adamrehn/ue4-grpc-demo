#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Copy the .proto file from the server source tree to the client source tree
cp ./server/CubePhysicsDemo/Source/CubePhysicsDemo/Proto/cube_demo.proto ./client/cube_demo.proto

# Build the Docker images
cd "$SCRIPT_DIR/compose/linux" && docker-compose build

# Run the Docker images
# (The client image will run and stop immediately, which is what we want in this case)
cd "$SCRIPT_DIR/compose/linux" && docker-compose up
