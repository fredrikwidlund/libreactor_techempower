# libreactor_techempower

Machine 1/2 (wrk) <- (4x10GbE) -> Machine 3 (16 x rest_server)

```
# ./wrk -c128 -d20s -t16 -s scripts/pipeline.lua http://x.x.x.x/plaintext
Running 20s test @ http://x.x.x.x/plaintext
  16 threads and 128 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   698.78us  624.70us  41.26ms   76.68%
    Req/Sec   772.96k    75.38k    1.40M    86.35%
  247255474 requests in 20.10s, 22.57GB read
Requests/sec: 12302056.29
Transfer/sec:      1.12GB
# ./wrk -c128 -d20s -t16 -s scripts/pipeline.lua http://x.x.x.x/plaintext
Running 20s test @ http://x.x.x.x/plaintext
  16 threads and 128 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   704.36us  564.73us  41.38ms   73.21%
    Req/Sec   758.38k    65.83k    1.17M    68.10%
  242452772 requests in 20.09s, 22.13GB read
Requests/sec: 12066253.00
Transfer/sec:      1.10GB
```

Total: 24+ Mrps

```
# ./wrk -c128 -d20s -t16 -s scripts/pipeline.lua http://x.x.x.x:8080/json
Running 20s test @ http://x.x.x.x:8080/json
  16 threads and 128 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   809.12us  624.57us  28.27ms   73.17%
    Req/Sec   698.49k    88.23k    0.94M    67.65%
  223436322 requests in 20.10s, 28.09GB read
Requests/sec: 11117006.48
Transfer/sec:      1.40GB
# ./wrk -c128 -d20s -t16 -s scripts/pipeline.lua http://x.x.x.x:8080/json
Running 20s test @ http://x.x.x.x:8080/json
  16 threads and 128 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   799.39us  615.46us  28.86ms   72.80%
    Req/Sec   711.03k    80.66k    1.30M    73.43%
  227381439 requests in 20.10s, 28.59GB read
Requests/sec: 11313420.67
Transfer/sec:      1.42GB
```

Total: 22+ Mrps
