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

## Plans

Current plans contains:
- Creating more effitient data-strucrure for storing key-value strings for cache
- Replasing current persistent storage with self-developed
- Creating benchmarks
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
- `max_request_length` Maximum possible summary length of command
- `buckets` Default number of buckets in hash table (can grow in runtime)
- `memory` Maximum available memory to the system. If memory is full, the majority of operations will result in errors
- `path` Path to persistent storage
- `create_if_missing` If `false` and database does not exist, result if failure

## Contributing

All contributions that address issues or implement features are welcome. If you encountered bugs or have some ideas you welcome to create an issue on github.
