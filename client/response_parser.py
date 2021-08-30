import struct
import enum
import collections


Response = collections.namedtuple('Response', ('version', 'status', 'filename', 'payload'))


class Status(enum.Enum):
    SuccessFileRecovered = 210
    SuccessFileListReceived = 211
    SuccessFileBackedUpOrDeleted = 212
    FailureFileNotFound = 1001
    FailureNoFilesForClient = 1002
    FailureGeneralError = 1003


class ResponseParser(object):
    @staticmethod
    def _parse_response_header(sock):
        response_header_format = '<BH'
        data = sock.recv(struct.calcsize(response_header_format))
        version, status = struct.unpack(response_header_format, data)
        return version, Status(status)

    @staticmethod
    def _parse_prefixed_string(sock, fmt):
        data = sock.recv(struct.calcsize(fmt))
        length, = struct.unpack(fmt, data)
        string = sock.recv(length)
        return string

    @staticmethod
    def _parse_filename(sock):
        return ResponseParser._parse_prefixed_string(sock, '<H')

    @staticmethod
    def _parse_payload(sock):
        return ResponseParser._parse_prefixed_string(sock, '<I')

    @staticmethod
    def parse_response(sock):
        version, status = ResponseParser._parse_response_header(sock)
        filename = None
        payload = None
        if status in [Status.SuccessFileRecovered, Status.SuccessFileListReceived,
                      Status.SuccessFileBackedUpOrDeleted, Status.FailureFileNotFound]:
            filename = ResponseParser._parse_filename(sock)
            if status in [Status.SuccessFileRecovered, Status.SuccessFileListReceived]:
                payload = ResponseParser._parse_payload(sock)

        return Response(version, status, filename, payload)
