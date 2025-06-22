### Build

```bash
./build.sh
```

### Run

```bash
./run.sh
```

### Result

```
❯ ./run.sh
Knapsack Problem (capacity: 50)
Items: (w:10, v:60) (w:20, v:100) (w:30, v:120) 

Recursive solution: 220
Dynamic Programming: 220
Space-optimized DP: 220

Greedy approximation (by value/weight ratio):
Taking item 0 (weight: 10, value: 60, ratio: 6.00)
Taking item 1 (weight: 20, value: 100, ratio: 5.00)
Greedy result: 160
``` 
