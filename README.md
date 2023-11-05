# Membase - key-value storage solution

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

## Commands

- `GET [key]`
- `PUT [key] [value]`
- `REMOVE [key]`
- `WIPE`
- `FINDKEY [search]`
- `FINDVALUE [search]`

## Contributing
