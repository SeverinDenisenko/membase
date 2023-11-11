#!/usr/bin/env python3

from framework import testing


class ErrorsTest(testing.Test):
    def tests(self) -> None:
        self.app.test('PUT', 'ERROR')
        self.app.test('PUT value', 'ERROR')
        self.app.test('GET value', 'ERROR')
        self.app.test('GET', 'ERROR')
        self.app.test('Hello', 'ERROR')
        self.app.test(' ', 'ERROR')
        self.app.test('\n', 'ERROR')
        self.app.test('\t\n', 'ERROR')
        self.app.test('\0', 'ERROR')
        self.app.test('!@#$%^&*()', 'ERROR')
        self.app.test('FINDKEY', 'ERROR')
        self.app.test('FINDVALUE', 'ERROR')
        self.app.test('WIP', 'ERROR')
        self.app.test('PU', 'ERROR')
        self.app.test('PUTT', 'ERROR')


ErrorsTest().run()
