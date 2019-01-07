#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "../Private/Generated/cube_demo.grpc.pb.h"

class GrpcServiceImp : public CubePhysicsDemo::Service
{
	public:
		GrpcServiceImp(AStaticMeshActor* cube);
		virtual grpc::Status ResetCubeTransform(grpc::ServerContext* context, const Empty* request, Empty* response);
		virtual grpc::Status GetCubeTransform(grpc::ServerContext* context, const Empty* request, Transform* response);
		virtual grpc::Status ApplyForceToCube(grpc::ServerContext* context, const Vector3D* request, Empty* response);
		
	private:
		AStaticMeshActor* cube;
};
