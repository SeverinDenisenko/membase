#!/bin/bash
set -eo pipefail

source $(dirname "$0")/framework.sh

run "$1" "$2"

test 'PUT 69 96' 'OK'
test 'GET 69' 'VALUE 96'
test 'PUT name severin' 'OK'
test 'GET name' 'VALUE severin'
test 'REMOVE name' 'OK'
test 'GET name' 'ERROR'
test 'GET 69' 'VALUE 96'
test 'PUT user:name:first severin' 'OK'
test 'GET user:name:first' 'VALUE severin'
test 'PUT basic:json {fist:"severin",work:"programmer",token:"!@#$%^&*()_+="}' 'OK'
test 'GET basic:json' 'VALUE {fist:"severin",work:"programmer",token:"!@#$%^&*()_+="}'
test 'WIPE' 'OK'
test 'GET basic:json' 'ERROR'

good

