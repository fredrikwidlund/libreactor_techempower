# libreactor_techempower

Machine 1 (wrk) <-(4x10GbE)-> Machine 2(16xrest_server)

    #  ./wrk -c 512 -d 10s -t 16 -s scripts/pipeline.lua http://10.24.133.32/plaintext
    Running 10s test @ http://10.24.133.32/plaintext
      16 threads and 512 connections
      Thread Stats   Avg      Stdev     Max   +/- Stdev
        Latency     2.75ms    3.38ms  55.30ms   80.85%
        Req/Sec     1.15M    84.80k    2.04M    92.42%
      183913730 requests in 10.10s, 13.19GB read
    Requests/sec: 18214226.89
    Transfer/sec:      1.31GB
