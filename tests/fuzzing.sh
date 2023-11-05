#!/bin/bash
set -eo pipefail

source $(dirname "$0")/framework.sh

run "$1" "$2"

for i in $(seq 1 100); do
    test "PUT $i $i" "OK"
done

for i in $(seq 1 100); do
    test "GET $i" "VALUE $i"
done

test 'WIPE' 'OK'

for i in $(seq 1 100); do
    test "GET $i" "ERROR"
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

