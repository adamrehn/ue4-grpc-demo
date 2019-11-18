UE4 gRPC Microservice Demo
==========================

The code in this repository demonstrates running an Unreal Engine-powered microservice in the Docker containers built by the [ue4-docker](https://github.com/adamrehn/ue4-docker) Python package. The client and server use Google's popular [gRPC](https://grpc.io/) framework for communication. **Note that this code is provided as an example only and is NOT designed for production use.**

## Contents

- [Prerequisites](#prerequisites)
- [Running the demo](#running-the-demo)
- [Explanation](#explanation)
- [Legal](#legal)


## Prerequisites

- Currently only Linux containers are supported (Windows container support will be added soon, although just for demonstration purposes since Linux containers are strongly recommended for running UE4 microservices.)
- [Docker](https://www.docker.com/) and [Docker Compose](https://docs.docker.com/compose/) 1.19.0 or newer
- [ue4-docker](https://github.com/adamrehn/ue4-docker) 0.0.39 or newer


## Running the demo

- Ensure you have built the Docker image `adamrehn/ue4-full:4.23.1` using [ue4-docker](https://github.com/adamrehn/ue4-docker) version 0.0.39 or newer. Assuming you have the package installed and the host system is configured correctly, you can build the image using the following command:
  
  ```
  ue4-docker build 4.23.1 --no-engine
  ```

- Start the server in a terminal by running the `./start-server.sh` shell script. Docker will build and run the images for the demo. This may take some time. You will know everything is built and running when you see a line in the log output that looks like this:
  
  ```
  LogLoad: Took 0.535869 seconds to LoadMap(/Game/Default)
  ```

- Run the interactive client in a second terminal by running the `./start-client.sh` shell script. The script communicates with the server to monitor the current transform of a cube in a blank scene with a floor beneath it. You can use the controls to reset the cube to its starting point above the floor or apply a random physics force to the cube, and watch the cube's transform update in real-time.

- Once you are done, hit the Q key in the second terminal to close the interactive client, and Ctrl-C in the first terminal to stop the server.


## Explanation

This demo shows off an extremely basic example of a UE4-powered microservice that makes use of the Engine's physics simulation capabilities. The demo project contains a scene with a floor and a single physics-enabled cube, along with a gRPC service that allows clients to query the state of the cube and perturb it so that the results of physics acting on the cube can be observed.

The implementation is quite straightforward:
- The infrastructure from [conan-ue4cli](https://github.com/adamrehn/conan-ue4cli) is used to build a version of gRPC and its dependencies that are compatible with the Unreal Engine. These libraries are statically linked and incur no additional runtime dependencies for the demo project once it is built.
- The [GrpcServerWrapper](./server/CubePhysicsDemo/Source/CubePhysicsDemo/Public/GrpcServerWrapper.h) Actor is placed in the scene and has its `cube` property set to reference the cube Actor.
- The wrapper is responsible for starting the gRPC server with an instance of the [GrpcServiceImp](./server/CubePhysicsDemo/Source/CubePhysicsDemo/Public/GrpcServiceImp.h) class, which contains the actual service implementation.
- The methods of the service implementation are run on a background thread spawned by gRPC, so each method must communicate asynchronously with the game thread in order to safely manipulate the state of the cube.

For the sake of simplicity (and support for configurations other than just Linux + NVIDIA hardware), the demo does not perform any rendering. However, most real-world use cases for UE4 microservices involve performing rendering in addition physics simulations. Cloud rendering support can be added quite easily so long as the server is run via NVIDIA Docker under Linux. See the [ue4-cloud-rendering-demo](https://github.com/adamrehn/ue4-cloud-rendering-demo) repository for an example of performing cloud rendering in Docker containers and streaming the results to a web browser via WebRTC.


## Legal

Copyright &copy; 2019, Adam Rehn. Licensed under the MIT License, see the file [LICENSE](./LICENSE) for details.
