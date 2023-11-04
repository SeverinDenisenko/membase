#!/bin/bash
set -eo pipefail

source $(dirname "$0")/framework.sh

run "$1" "$2"

test 'PUT' 'ERROR'
test 'GET' 'ERROR'
test 'PUT 69' 'ERROR'
test 'Hello' 'ERROR'
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

for i in $(seq 1000 1100); do
    test "PUT $i $i" "OK"
done

for i in $(seq 1000 1100); do
    test "GET $i" "VALUE $i"
done

for i in $(seq 1 100); do
    str1=$(openssl rand -hex 100)
    str2=$(openssl rand -hex 200)
    str3=$(openssl rand -hex 300)
    str4=$(openssl rand -hex 400)
    key1=$(openssl rand -hex 100)
    key2=$(openssl rand -hex 200)
    key3=$(openssl rand -hex 300)
    key4=$(openssl rand -hex 400)
    test "PUT $key4 $str1" "OK"
    test "PUT $key3 $str2" "OK"
    test "PUT $key2 $str3" "OK"
    test "PUT $key1 $str4" "OK"
    test "GET $key4" "VALUE $str1"
    test "GET $key3" "VALUE $str2"
    test "GET $key2" "VALUE $str3"
    test "GET $key1" "VALUE $str4"
done

good

