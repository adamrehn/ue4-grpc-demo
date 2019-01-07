#!/usr/bin/env bash
docker run --rm -ti --network=host ue4-grpc-demo/client:latest python3 /tmp/client.py
