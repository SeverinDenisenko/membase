#!/usr/bin/env python3

from framework import testing
import random
import string


class BasicTest(testing.Test):
    def tests(self) -> None:
        for i in range(100):
            self.app.test(f'PUT {i} {i}', 'OK')

        for i in range(100):
            self.app.test(f'GET {i}', f'VALUE {i}')

        self.app.test('WIPE', 'OK')

        for i in range(100):
            self.app.test(f'GET {i}', f'ERROR')

        data = {}
        req_len = 1024

        for i in range(100):
            key = ''.join(random.choices(
                string.ascii_uppercase + string.digits, k=req_len))
            value = ''.join(random.choices(
                string.ascii_uppercase + string.digits, k=req_len))
            data[key] = value
            self.app.test(f'PUT {key} {value}', f'OK')

        for key, value in data.items():
            self.app.test(f'GET {key}', f'VALUE {value}')


BasicTest().run()
