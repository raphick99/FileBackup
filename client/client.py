import socket
from request_builder import RequestBuilder
from response_parser import ResponseParser
from config import VERSION


class Client(object):
    def __init__(self, environment):
        self.environment = environment
        self.sock = self.initiate_client_socket(self.environment.ip, self.environment.port)

    @staticmethod
    def initiate_client_socket(ip, port):
        sock = socket.socket()
        sock.connect((ip, port))
        return sock

    def _send_request(self, op, filename, payload):
        self.sock.send(RequestBuilder.build_request(self.environment.user_id, VERSION, op, filename, payload))

    def _receive_response(self):
        return ResponseParser.parse_response(self.sock)

    def execute_request(self, op, filename=None, payload=None):
        self._send_request(op, filename, payload)
        return self._receive_response()
