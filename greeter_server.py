from concurrent import futures
import time
import logging

import grpc

import helloworld_pb2
import helloworld_pb2_grpc


class Server(helloworld_pb2_grpc.ServerServicer):
    def __init__(self):
        self.chats = 0

    def StreamCcu(self, request, context):
        self.chats += 1
        num = -1
        try:
            while context.is_active():
                self_chats = self.chats
                if self_chats != num:
                    num = self.chats
                    yield helloworld_pb2.CcuReply(n=num)
                else:
                    time.sleep(0.01)
        finally:
            self.chats -= 1

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10000))
    helloworld_pb2_grpc.add_ServerServicer_to_server(Server(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()
