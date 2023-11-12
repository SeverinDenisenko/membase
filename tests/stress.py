#!/usr/bin/env python3

from framework import testing
import random
import string


class StressTest(testing.Test):
    def tests(self) -> None:
        data = []
        req_len = 1024
        req_count = 1024 * 256

        for i in range(req_count):
            key = ''.join(random.choices(
                string.ascii_uppercase + string.digits, k=req_len))
            value = ''.join(random.choices(
                string.ascii_uppercase + string.digits, k=req_len))
            data.append(f'PUT {key} {value}')

        self.app.test_seq(data, 'OK')


StressTest().run()
