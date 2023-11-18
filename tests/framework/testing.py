import subprocess
from subprocess import Popen
import socket
import time
import os
import signal
import argparse
from pathlib import Path


class Membase:
    def __init__(self, executable: str, config: str, ip: str, port: int) -> None:
        self.args = [executable, '-config', config]
        self.ip = ip
        self.port = port

    def prepare(self) -> None:
        self.process = Popen(self.args)

    def done(self) -> None:
        self.process.kill()

    def send(self, input: str) -> str:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.ip, self.port))
        s.sendall(input.encode())
        data = s.recv(4096)
        s.close()
        return data.decode()[:-1]

    def test_seq(self, input: list, expected: str) -> None:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.ip, self.port))
        for elem in input:
            s.sendall(elem.encode())
            response = s.recv(4096).decode()[:-1]
            if response == expected:
                print(f"OK: {response} == {expected}")
            else:
                print(f"FAIL: {response} != {expected}")
                self.done()
                exit(1)
        s.close()

    def test(self, input: str, expected: str) -> None:
        print(f"RUNNING: {input}")
        response = self.send(input)
        if response == expected:
            print(f"OK: {response} == {expected}")
        else:
            print(f"FAIL: {response} != {expected}")
            self.done()
            exit(1)


class Test:
    def __init__(self) -> None:
        parser = argparse.ArgumentParser()
        parser.add_argument('executable', type=Path)
        parser.add_argument('config', type=Path)
        args = parser.parse_args()
        self.port = 2222
        self.ip = '127.0.0.1'
        self.config = args.config.absolute()
        self.executable = args.executable.absolute()

    def run(self) -> None:
        try:
            self.app = Membase(self.executable, self.config,
                               self.ip, self.port)
            self.app.prepare()
            time.sleep(3)
            self.tests()
            self.app.done()
        except Exception as e:
            print(f"FAIL: {e}")
            self.app.done()
            exit(1)

    def tests(self) -> None:
        pass
