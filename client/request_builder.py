import struct
import enum


class Op(enum.Enum):
    BackupFile = 100
    RecoverFile = 200
    DeleteBackedUpFile = 201
    ListFiles = 202


class RequestBuilder(object):
    @staticmethod
    def _build_request_header(user_id, version, op):
        return struct.pack('<IBB', user_id, version, op.value)

    @staticmethod
    def _build_prefixed_string(fmt, string):
        return struct.pack(fmt, len(string)) + bytes(string, 'utf-8')

    @staticmethod
    def _build_filename(filename):
        return RequestBuilder._build_prefixed_string('<H', filename)

    @staticmethod
    def _build_payload(payload):
        return RequestBuilder._build_prefixed_string('<I', payload)

    @staticmethod
    def build_request(user_id, version, op, filename=None, payload=None):
        request = RequestBuilder._build_request_header(user_id, version, op)
        if filename:
            request += RequestBuilder._build_filename(filename)
        if payload:
            request += RequestBuilder._build_payload(payload)

        return request
