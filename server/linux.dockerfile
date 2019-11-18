FROM adamrehn/ue4-full:4.23.1-opengl as builder

# Build the Conan packages for the demo's dependencies
RUN ue4 conan update
RUN ue4 conan build grpc-ue4

# Copy the demo project source code and perform a clean
COPY --chown=ue4:ue4 CubePhysicsDemo /tmp/CubePhysicsDemo
WORKDIR /tmp/CubePhysicsDemo
RUN ue4 clean

# Pre-generate our Protobuf and gRPC source files
# (This ensures UBT will always detect them correctly regardless of UE4 version)
RUN conan install . --profile ue4

# Package the demo project
RUN ue4 package Development

# Copy the packaged project to a runtime container
FROM adamrehn/ue4-runtime:latest
COPY --from=builder /tmp/CubePhysicsDemo/dist/LinuxNoEditor /home/ue4/CubePhysicsDemo

# Expose the TCP port that the gRPC server will listen on
EXPOSE 50051

# Setup our entrypoint
ENTRYPOINT ["/home/ue4/CubePhysicsDemo/CubePhysicsDemo.sh", "-nullrhi", "-nosplash"]
