#!/usr/bin/env python3

from framework import testing

class BasicTest(testing.Test):
    def tests(self) -> None:
        self.app.test('PUT 69 96', 'OK')
        self.app.test('GET 69', 'VALUE 96')
        self.app.test('PUT name severin', 'OK')
        self.app.test('GET name', 'VALUE severin')
        self.app.test('REMOVE name', 'OK')
        self.app.test('GET name', 'ERROR')
        self.app.test('GET 69', 'VALUE 96')
        self.app.test('PUT user:name:first severin', 'OK')
        self.app.test('GET user:name:first', 'VALUE severin')
        self.app.test('PUT basic:json {fist:"severin",work:"programmer",token:"!@#$%^&*()_+="}', 'OK')
        self.app.test('GET basic:json', 'VALUE {fist:"severin",work:"programmer",token:"!@#$%^&*()_+="}')
        self.app.test('WIPE', 'OK')
        self.app.test('GET basic:json', 'ERROR')


BasicTest().run()
