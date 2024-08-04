### Result

```
./build.sh
./run.sh
```

```
❯ curl -i http://127.0.0.1:8080
HTTP/1.1 200 OK
Content-Type: text/plain
Content-Length: 13

Hello, World!%
```

### Stress Test

```
ab -k -c 1000 -n 10000 http://localhost:8080/
```

```
ab -k -c 1000 -n 10000 http://localhost:8080/
This is ApacheBench, Version 2.3 <$Revision: 1879490 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            8080

Document Path:          /
Document Length:        13 bytes

Concurrency Level:      1000
Time taken for tests:   0.698 seconds
Complete requests:      10000
Failed requests:        0
Keep-Alive requests:    0
Total transferred:      780000 bytes
HTML transferred:       130000 bytes
Requests per second:    14323.32 [#/sec] (mean)
Time per request:       69.816 [ms] (mean)
Time per request:       0.070 [ms] (mean, across all concurrent requests)
Transfer rate:          1091.03 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    4   8.7      0      42
Processing:     0    4   6.9      0      42
Waiting:        0    3   5.1      0      23
Total:          1    7  14.8      1      57

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      6
  90%     28
  95%     53
  98%     54
  99%     54
 100%     57 (longest request)
```