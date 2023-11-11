#!/bin/bash
set -eo pipefail

run () {
    executable="$1"
    config="$2"

    nohup "$executable" -config "$config" >/dev/null &
    sleep 3
}

fatal () {
    echo "Some tests failed"
    kill -s SIGKILL $(pgrep membase)
    exit 1
}

good () {
    echo "All tests passed"
    kill -s SIGKILL $(pgrep membase)
    exit 0
}

send () {
    echo "Running $1"
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    	retval=$(echo "$1" | nc -N 127.0.0.1 2222)
    else
        retval=$(echo "$1" | nc -N 0 127.0.0.1 2222)
    fi
}

check () {
    if [ "$1" = "$2" ]
    then
        echo "Success: $1 == $2"
    else
        echo "Failed: $1 != $2"
        fatal
    fi
}

test () {
    send "$1"
    check "$retval" "$2"
}

