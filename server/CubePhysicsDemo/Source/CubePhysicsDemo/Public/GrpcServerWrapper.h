#pragma once

#include <grpc++/grpc++.h>
#include "CoreMinimal.h"
#include "GrpcServiceImp.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "GrpcServerWrapper.generated.h"

UCLASS()
class CUBEPHYSICSDEMO_API AGrpcServerWrapper : public AActor
{
	GENERATED_BODY()
	
	AGrpcServerWrapper();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	public:
		
		//The cube that we are controlling
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CubePhysicsDemo)
		AStaticMeshActor* cube;
		
		//The TCP port that the gRPC server will bind to
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CubePhysicsDemo)
		int port;
	
	private:
		
		//The gRPC service implementation and server object
		TUniquePtr<GrpcServiceImp> service;
		TUniquePtr<grpc::Server> server;
};
