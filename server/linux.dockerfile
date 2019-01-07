FROM adamrehn/ue4-full:4.21.1

# Build the Conan packages for the demo's dependencies
WORKDIR /home/ue4/ue4-conan-recipes
RUN git pull
RUN ue4 conan build grpc-ue4

# Build the demo project and pre-fill the DDC
COPY --chown=ue4:ue4 CubePhysicsDemo /tmp/CubePhysicsDemo
WORKDIR /tmp/CubePhysicsDemo
RUN ue4 build
RUN ue4 run -run=DerivedDataCache -fill -projectonly

# Expose the TCP port that the gRPC server will listen on
EXPOSE 50051

# Setup our entrypoint
ENTRYPOINT ["ue4", "run", "-game", "-nullrhi", "-nosplash"]
