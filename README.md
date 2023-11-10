# Membase - key-value storage solution

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
- Replasing current testing solution with faster and more flexible
- Creating benchmarks
- Creating flexible search functionality
- Adding wrapper libraries for different programming languages (C, C++, Python, Rust, Go, are priorities for now)

## Commands

- `GET [key]` 
- `PUT [key] [value]`
- `REMOVE [key]` Removes key form storage
- `WIPE` Completely erases database
- `FINDKEY [search]` Returns list of keys that start with specific sequence
- `FINDVALUE [search]` Returns list of keys, values of such starts with specific sequence

## Contributing

All contributions that address issues or implement features are welcome. If you encountered bugs or have some ideas you welcome to create an issue on github.
