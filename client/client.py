import socket
from request_builder import RequestBuilder
from response_parser import ResponseParser
from config import VERSION


class Client(object):
    def __init__(self, environment):
        self.environment = environment
        self.current_sock = None

    @staticmethod
    def initiate_client_socket(ip, port):
        sock = socket.socket()
        sock.connect((ip, port))
        return sock

    def _send_request(self, op, filename, payload):
        self.current_sock.send(RequestBuilder.build_request(self.environment.user_id, VERSION, op, filename, payload))

    def _receive_response(self):
        return ResponseParser.parse_response(self.current_sock)

    def execute_request(self, op, filename=None, payload=None):
        self.current_sock = self.initiate_client_socket(self.environment.ip, self.environment.port)
        self._send_request(op, filename, payload)
        response = self._receive_response()
        self.current_sock.close()
        return response
