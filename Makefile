CXX = g++ --std=c++17 -Wall
LDFLAGS = -lpthread `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -ldl

all: helloworld_pb2.py helloworld_pb2_grpc.py greeter_client greeter_server

greeter_client: helloworld.pb.o helloworld.grpc.pb.o greeter_client.o
	$(CXX) $^ $(LDFLAGS) -o $@

greeter_server: helloworld.pb.o helloworld.grpc.pb.o greeter_server.o
	$(CXX) $^ $(LDFLAGS) -o $@

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	protoc -I. --cpp_out=. $<

helloworld_pb2.py: helloworld.proto
	python -m grpc_tools.protoc -I. --python_out=. helloworld.proto

helloworld_pb2_grpc.py: helloworld.proto
	python -m grpc_tools.protoc -I. --grpc_python_out=. helloworld.proto
