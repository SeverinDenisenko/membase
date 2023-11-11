#!/bin/bash
set -eo pipefail

source $(dirname "$0")/framework.sh

run "$1" "$2"

test 'PUT' 'ERROR'
test 'PUT value' 'ERROR'
test 'GET value' 'ERROR'
test 'GET' 'ERROR'
test 'Hello' 'ERROR'
test '' 'ERROR'
test '!@#$%^&*()' 'ERROR'
test 'FINDKEY' 'ERROR'
test 'FINDVALUE' 'ERROR'
test 'WIP' 'ERROR'
test 'PU' 'ERROR'
test 'PUTT' 'ERROR'

good

