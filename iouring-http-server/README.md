### Install dependencies

```
sudo apt-get install -y liburing-dev
```

### Result

```
./build.sh
./run.sh
```

```
❯ curl -i http://127.0.0.1:8080
HTTP/1.1 200 OK
Content-Length: 13

Hello, World!%
```

### Stress test

```
ab -k -c 1000 -n 10000 http://localhost:8080/
```

```
❯ ab -k -c 1000 -n 10000 http://localhost:8080/
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
Time taken for tests:   0.591 seconds
Complete requests:      10000
Failed requests:        0
Keep-Alive requests:    0
Total transferred:      520000 bytes
HTML transferred:       130000 bytes
Requests per second:    16917.33 [#/sec] (mean)
Time per request:       59.111 [ms] (mean)
Time per request:       0.059 [ms] (mean, across all concurrent requests)
Transfer rate:          859.08 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    3   2.0      3      22
Processing:     1    4   0.7      4      22
Waiting:        0    3   0.8      3       5
Total:          5    8   2.0      7      26

Percentage of the requests served within a certain time (ms)
  50%      7
  66%      8
  75%      8
  80%      8
  90%      8
  95%      8
  98%      8
  99%     24
 100%     26 (longest request)
```