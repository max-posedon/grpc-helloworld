#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <experimental/random>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;
using helloworld::Empty;
using helloworld::CcuReply;
using helloworld::Server;


class Client 
{
    public:
        Client(std::shared_ptr<Channel> channel) : stub_(Server::NewStub(channel)) {};

    ClientContext context;

    void StreamCcu() 
    {
        Empty request;
        CcuReply reply;

        std::unique_ptr<ClientReader<CcuReply> > reader(stub_->StreamCcu(&context, request));
        while (reader->Read(&reply)) {
            std::cout << "Greeter received: " << reply.n() << std::endl;
        }
        Status status = reader->Finish();
    }

    private:
        std::unique_ptr<Server::Stub> stub_;
};


int main(int argc, char** argv)
{
    ChannelArguments args;
    Client client(grpc::CreateCustomChannel("localhost6:50051", grpc::InsecureChannelCredentials(), args));
  
    std::this_thread::sleep_for(std::chrono::seconds(std::experimental::randint(1,5)));
    
    std::thread t([](Client *srv) {
        srv->StreamCcu(); 
    }, &client);
    
    std::this_thread::sleep_for(std::chrono::seconds(std::experimental::randint(1,5)));
    
    client.context.TryCancel();
    t.join();
    return 0;
}
