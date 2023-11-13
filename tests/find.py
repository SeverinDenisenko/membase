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
        
        self.app.test('WIPE', 'OK')

        self.app.test('PUT 1234567890 qwerty', 'OK')
        self.app.test('PUT 123456789 qwerty', 'OK')
        self.app.test('PUT 12345678 qwerty', 'OK')
        self.app.test('PUT 1234567 qwerty', 'OK')
        self.app.test('PUT 123456 qwerty', 'OK')
        self.app.test('PUT 12345 qwerty', 'OK')

        self.app.test('FINDVALUE qwerty',
                      'KEY 12345\nKEY 123456\nKEY 1234567\nKEY 12345678\nKEY 123456789\nKEY 1234567890\nOK')
        self.app.test('FINDKEY 12345678',
                      'KEY 12345678\nKEY 123456789\nKEY 1234567890\nOK')
        
        self.app.test('WIPE', 'OK')

        self.app.test('PUT walk 1', 'OK')
        self.app.test('PUT want 2', 'OK')
        self.app.test('PUT war 3', 'OK')
        self.app.test('PUT warp 4', 'OK')

        self.app.test('FINDKEY wa',
                      'KEY walk\nKEY want\nKEY war\nKEY warp\nOK')


FindTest().run()
