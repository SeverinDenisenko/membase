#!/bin/bash
set -eo pipefail

source $(dirname "$0")/framework.sh

run "$1" "$2"

key=$(cat <<EOF
KEY 1234567890
OK
EOF)

value=$(cat <<EOF
KEY 1234567890
OK
EOF)

test 'PUT 1234567890 qwertyuiop' 'OK'
test 'FINDKEY 1234' "$key"
test 'FINDVALUE qwer' "$value"

test 'WIPE' 'OK'

key=$(cat <<EOF
KEY 1234567890
OK
EOF)

value=$(cat <<EOF
KEY 1234567890
OK
EOF)

test 'PUT 1234567890 qwerty' 'OK'
test 'PUT hello world' 'OK'
test 'PUT !@#$%^ meaw' 'OK'
test 'PUT Kavai UwU' 'OK'
test 'FINDKEY 1234567890' "$key"
test 'FINDVALUE qwerty' "$value"

good

