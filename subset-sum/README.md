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
Subset Sum Problem
Array: 3 34 4 12 5 2 
Target sum: 9

=== Testing if subset exists ===
Memoized recursive: Subset exists
Dynamic Programming: Subset exists
Space-optimized DP: Subset exists

=== Finding actual subset ===
One possible subset: { 2 4 3 }

=== Finding all subsets ===
Subset found: { 3 4 2 }
Subset found: { 4 5 }

=== Greedy approximation ===
Greedy approximation: 5 4 
Sum achieved: 9 (target: 9)
``` 
