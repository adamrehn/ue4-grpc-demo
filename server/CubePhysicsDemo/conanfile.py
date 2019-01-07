from conans import ConanFile
import os

class CubePhysicsDemoConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "json"
    requires = (
        "grpc-ue4/1.16.0@adamrehn/4.21"
    )
    
    def imports(self):
        
        # Generate the source files for our gRPC service definitions
        from grpc_helper import ProtoCompiler
        compiler = ProtoCompiler(self.deps_user_info["grpc-ue4"].build_data)
        sourceDir = os.path.join(os.path.dirname(__file__), "Source", "CubePhysicsDemo")
        protos = [os.path.join(sourceDir, "Proto", "cube_demo.proto")]
        outdir = os.path.join(sourceDir, "Private", "Generated")
        compiler.codegen(protos, outdir)
