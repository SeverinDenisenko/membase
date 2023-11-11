#!/usr/bin/env python3

from framework import testing


class FindTest(testing.Test):
    def tests(self) -> None:
        self.app.test('PUT 1234567890 qwertyuiop', 'OK')
        self.app.test('FINDKEY 1234',
                      'KEY 1234567890\nOK')
        self.app.test('FINDVALUE qwer',
                      'KEY 1234567890\nOK')

        self.app.test('WIPE', 'OK')

        self.app.test('PUT 1234567890 qwerty', 'OK')
        self.app.test('PUT hello world', 'OK')
        self.app.test('PUT !@#$%^ meaw', 'OK')
        self.app.test('PUT Kavai UwU', 'OK')
        self.app.test('FINDKEY 1234567890',
                      'KEY 1234567890\nOK')
        self.app.test('FINDVALUE qwerty',
                      'KEY 1234567890\nOK')


FindTest().run()
