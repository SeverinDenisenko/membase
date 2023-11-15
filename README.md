# Membase – key-value storage solution

## Why this exists?

The goal of this project is to provide simple, easy to modify and relatively fast key-value storage, written in modern C++. While alternatives like Memcached and Redis written in pure C, this project focuses on code readability and flexibility.

## How to use

You can insert values using `PUT`:

```
PUT user:47:name severin
OK
PUT user:47:id 47
OK
PUT user:47:age 21
OK
```

Then You can search:

```
FINDKEY user:47
KEY user:47:name
KEY user:47:age
KEY user:47:id
OK
```

Then `GET`:

```
GET user:47:age
VALUE 21
```

See section "Commands" for more.

## Features

### Modes

Database can operate in different modes:
- memory: store data only in-memory
- persistent: store data only on disk

### Performanse

Stats for `memory` mode:

```
... <= 0.1 ms 0
0.1 <= 0.2 ms 0
0.2 <= 0.3 ms 0
0.3 <= 0.4 ms 0
0.4 <= 0.5 ms 0
0.5 <= 0.6 ms 1803
0.6 <= 0.7 ms 97549
0.7 <= 0.8 ms 534
0.8 <= 0.9 ms 56
0.9 <= 1.0 ms 10
1.0 <= 1.1 ms 8
1.1 <= 1.2 ms 4
1.2 <= 1.3 ms 0
1.3 <= 1.4 ms 0
1.4 <= 1.5 ms 2
1.5 <= ... ms 34
Samples:  100000
Min latency:  0.568
Max latency:  4.246
Mean latency:  0.63423
```

Stats for `persistent` mode:

```
... <= 1.5 ms 0
1.5 <= ... ms 5000
Samples:  5000
Min latency:  1.759
Max latency:  18.097
Mean latency:  5.5521426
```

## Plans

Current plans contains:
- Creating more effitient data-strucrure for storing key-value strings for cache
- Replasing current persistent storage with self-developed
- Creating flexible search functionality
- Adding wrapper libraries for different programming languages (C, C++, Python, Rust, Go, are priorities for now)

## Commands

- `GET [key]` Returns the value if exists, error otherwise
- `PUT [key] [value]` Insert pair, key and value can be any sequence of symbols (not control symbols or spaces)
- `REMOVE [key]` Removes key form storage
- `WIPE` Completely erases database
- `FINDKEY [search]` Returns list of keys that start with specific sequencee in lexical order
- `FINDVALUE [search]` Returns list of keys, values of such starts with specific sequence in lexical order

## Config

You have to provide app with argument `-config config.json`. Example config:

```json
{
    "mode": "persistent",
    "server": {
        "host": "127.0.0.1",
        "port": 2222,
        "max_request_length": 4098
    },
    "cache": {
        "buckets": 1048576,
        "memory": 1073741824
    },
    "persistent": {
        "path": "/tmp/testdb",
        "create_if_missing": true
    }
}
```

Where:

- `mode` Can be `persistent` or `memory`
- `server`
- - `host` & `port` Accept connections from
- - `max_request_length` Maximum possible summary length of command
- `cache`
- - `buckets` Default number of buckets in hash table (can grow in runtime)
- - `memory` Maximum available memory to the system. If memory is full, the majority of operations will result in errors
- `persistent`
- - `path` Path to persistent storage
- - `create_if_missing` If `false` and database does not exist, result if failure

## Contributing

All contributions that address issues or implement features are welcome. If you encountered bugs or have some ideas you welcome to create an issue on github.
