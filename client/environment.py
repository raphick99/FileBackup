import ipaddress
import random
from config import *


class Environment(object):
    def __init__(self):
        self.user_id = self._generate_user_id()
        self.ip, self.port = self._parse_server_file()
        self.backed_up_files = self._parse_backup_file()

    @staticmethod
    def _generate_user_id():
        return random.randint(0, 0xffffffff)

    @staticmethod
    def _parse_server_file():
        with open(SERVER_FILE, 'r') as server_file:
            ip, port = server_file.read().split(sep=':', maxsplit=1)
            port = int(port)  # convert from string to integer
            Environment._validate_network_address(ip, port)
        return ip, port

    @staticmethod
    def _parse_backup_file():
        with open(BACKUP_FILE, 'r') as backup_file:
            return [f.strip() for f in backup_file.readlines()]

    @staticmethod
    def _validate_network_address(ip, port):
        ipaddress.ip_address(ip)  # raise exception if IP is invalid
        if port < 0 or port > 2**16:  # make sure is in valid range
            raise ValueError(f'port isnt in the right range. should be between 0 to 2**16. is: {port}')
