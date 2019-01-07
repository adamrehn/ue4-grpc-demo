#include "GrpcServiceImp.h"
#include "ThreadingHelpers.h"

GrpcServiceImp::GrpcServiceImp(AStaticMeshActor* cube) {
	this->cube = cube;
}

grpc::Status GrpcServiceImp::ResetCubeTransform(grpc::ServerContext* context, const Empty* request, Empty* response)
{
	//Reset the cube's transform on the game thread
	TFuture<void> result = AsyncNamed<void>(ENamedThreads::GameThread, [this]()
	{
		this->cube->SetActorLocationAndRotation(
			FVector(0.0f, 0.0f, 1000.0f),
			FRotator(),
			false,
			nullptr,
			ETeleportType::ResetPhysics
		);
	});
	
	//Block the gRPC thread until the lamda completes
	result.Get();
	return grpc::Status::OK;
}

grpc::Status GrpcServiceImp::GetCubeTransform(grpc::ServerContext* context, const Empty* request, Transform* response)
{
	//Retrieve the current transform for the cube on the game thread
	TFuture<FTransform> result = AsyncNamed<FTransform>(ENamedThreads::GameThread, [this]() {
		return this->cube->GetTransform();
	});
	
	//Block the gRPC thread until the lamda completes
	FTransform transform = result.Get();
	
	//Copy the location vector into the response message
	FVector location = transform.GetLocation();
	response->mutable_position()->set_x(location.X);
	response->mutable_position()->set_y(location.Y);
	response->mutable_position()->set_z(location.Z);
	
	//Copy the rotation euler angles into the response message
	FVector eulerAngles = transform.GetRotation().Euler();
	response->mutable_rotation()->set_x(eulerAngles.X);
	response->mutable_rotation()->set_y(eulerAngles.Y);
	response->mutable_rotation()->set_z(eulerAngles.Z);
	
	//Copy the scale vector into the response message
	FVector scale = transform.GetScale3D();
	response->mutable_scale()->set_x(scale.X);
	response->mutable_scale()->set_y(scale.Y);
	response->mutable_scale()->set_z(scale.Z);
	
	return grpc::Status::OK;
}

grpc::Status GrpcServiceImp::ApplyForceToCube(grpc::ServerContext* context, const Vector3D* request, Empty* response)
{
	//Apply the specified physics force to the cube on the game thread
	FVector force(request->x(), request->y(), request->z());
	TFuture<void> result = AsyncNamed<void>(ENamedThreads::GameThread, [this, force]() {
		this->cube->GetStaticMeshComponent()->AddImpulse(force);
	});
	
	//Block the gRPC thread until the lamda completes
	result.Get();
	return grpc::Status::OK;
}
