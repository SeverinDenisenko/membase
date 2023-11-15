## Collect Membase performanse metrics

Run `go run src/main.go` with Membase started on 127.0.0.1:2222

Current stats:

```
Testing PUT...
.... ... 0.50 ms:  0
0.50 ... 0.75 ms:  97079
0.75 ... 1.00 ms:  1716
1.00 ... 1.25 ms:  483
1.25 ... 1.50 ms:  189
1.50 ... 1.75 ms:  127
1.75 ... 2.00 ms:  101
2.00 ... 2.25 ms:  104
2.25 ... 2.50 ms:  56
2.50 ... 2.75 ms:  35
2.75 ... 3.00 ms:  26
3.00 ... 3.25 ms:  16
3.25 ... 3.50 ms:  13
3.50 ... 3.75 ms:  7
3.75 ... 4.00 ms:  4
4.00 ... 4.25 ms:  5
4.25 ... 4.50 ms:  1
4.50 ... 4.75 ms:  3
4.75 ... 5.00 ms:  3
5.00 ... .... ms:  32
Samples: 100000
Min latency: 0.516
Max latency: 22.462
Mean latency: 0.610
Testing GET...
.... ... 0.50 ms:  0
0.50 ... 0.75 ms:  99715
0.75 ... 1.00 ms:  118
1.00 ... 1.25 ms:  37
1.25 ... 1.50 ms:  25
1.50 ... 1.75 ms:  18
1.75 ... 2.00 ms:  13
2.00 ... 2.25 ms:  11
2.25 ... 2.50 ms:  4
2.50 ... 2.75 ms:  3
2.75 ... 3.00 ms:  3
3.00 ... 3.25 ms:  1
3.25 ... 3.50 ms:  5
3.50 ... 3.75 ms:  1
3.75 ... 4.00 ms:  2
4.00 ... 4.25 ms:  1
4.25 ... 4.50 ms:  1
4.50 ... 4.75 ms:  0
4.75 ... 5.00 ms:  1
5.00 ... .... ms:  41
Samples: 100000
Min latency: 0.304
Max latency: 27.277
Mean latency: 0.367
```

Rare bursts of latency is a problem and i'm trying to fix this.
