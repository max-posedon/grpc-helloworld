import random
import time
import threading
import logging

import grpc

import helloworld_pb2
import helloworld_pb2_grpc

class Client:
    
    def __init__(self):
        channel = grpc.insecure_channel('localhost:50051')
        self.stub = helloworld_pb2_grpc.ServerStub(channel)

    def run(self):
        threading.Thread(target=self.__stream_ccu, daemon=True).start()

    def __stream_ccu(self):
        responses = self.stub.StreamCcu(helloworld_pb2.Empty())
        for response in responses:
            print("Greeter client received: %d" % response.n)
    
if __name__ == '__main__':
    logging.basicConfig()
    client = Client()
    time.sleep(random.randint(1,5))
    client.run()
    time.sleep(random.randint(1,5))
