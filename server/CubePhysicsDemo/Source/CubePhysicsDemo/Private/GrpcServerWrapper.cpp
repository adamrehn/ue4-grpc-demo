#include "GrpcServerWrapper.h"

AGrpcServerWrapper::AGrpcServerWrapper()
{
	PrimaryActorTick.bCanEverTick = false;
	this->cube = nullptr;
	this->port = 50051;
}

void AGrpcServerWrapper::BeginPlay()
{
	Super::BeginPlay();
	
	//If we have a cube to control, start the gRPC server
	if (this->cube != nullptr)
	{
		//Instantiate our gRPC service implementation
		this->service.Reset(new GrpcServiceImp(this->cube));
		
		//Create and start the gRPC server
		grpc::ServerBuilder builder;
		builder.AddListeningPort("0.0.0.0:" + std::to_string(this->port), grpc::InsecureServerCredentials());
		builder.RegisterService(this->service.Get());
		this->server.Reset(builder.BuildAndStart().release());
	}
}

void AGrpcServerWrapper::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	//If the gRPC server was running, stop it
	if (this->server.IsValid() == true)
	{
		this->server->Shutdown();
		this->server.Reset(nullptr);
	}
}
