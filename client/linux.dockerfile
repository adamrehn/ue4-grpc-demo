FROM adamrehn/ue4-build-prerequisites:opengl

# Copy the client scripts
COPY --chown=ue4:ue4 cube_demo.proto client.py requirements.txt /tmp/

# Install the client's Python dependencies
USER root
RUN pip3 install -r /tmp/requirements.txt

# Run the protobuf compiler to generate the protobuf/gRPC code for the client
USER ue4
WORKDIR /tmp
RUN python3 -m grpc_tools.protoc --proto_path=/tmp --python_out=/tmp --grpc_python_out=/tmp /tmp/cube_demo.proto
