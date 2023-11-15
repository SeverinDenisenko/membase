## Collect embase performanse metrics

Run `go run src/main.go` with Membase started on 127.0.0.1:2222

Common parameters:

- `batches` How many batches will run
- `batch` Requests in batch
- `key & value length` Is 1024 by default

Total number of samples in `batches * batch`
