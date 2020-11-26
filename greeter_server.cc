#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using helloworld::Empty;
using helloworld::CcuReply;
using helloworld::Server;

// Logic and data behind the server's behavior.
class ServerServiceImpl final : public Server::Service {
    Status StreamCcu(ServerContext* context, const Empty* empty, ServerWriter<CcuReply>* writer) override 
    {
        chats += 1; 
        int num = -1;
        while(!context->IsCancelled()) {
            if (num != chats) {
                num = chats;
                CcuReply c;
                c.set_n(num);
                writer->Write(c);
            } else {
		        std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }

        chats -= 1;
        return Status::OK;
    }
  
    private:
        static int chats;
};

int ServerServiceImpl::chats = 0;

void RunServer() 
{
    std::string server_address("0.0.0.0:50051");
    
    ServerServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv)
{
    RunServer();
    return 0;
}
