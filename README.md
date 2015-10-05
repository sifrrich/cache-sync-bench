# cache-sync-bench
## 
Comparison of synchronisation via L1 or L3 cache. Two threads are created and assigned to different logical proccessors, which are either located on a single core (L1 sync) or different cores, connected via L3.

## Usage

To find out, which logical CPUs are physically located on the same core, refer to

`awk '/^domain/ { print $1, $2; } /^cpu/ { print $1; }' /proc/schedsta`

or

`cat /proc/cpuinfo|egrep 'processor|physical id|core id' |sed 's/^processor/\nprocessor/g'`

and set these numbers in `benc.c` using both makros `SMT0|1, CORE0|1`.

The gcc intrinsic ``sync_add_and_fetch()`` results in 

`f0 83 05 e0 16 20 00 	lock addl $0x1,0x2016e0(%rip)`,

thus the counter is only written (no L1/LLC loads, see [Results](#results)).

## Build
To build the benchmark on linux, only pthreads is needed. Run

`make`


## Results<a name="results"></a>

`make run` starts the benchmark using perf to get information on cache hits and run time.

An **Intel(R) Core(TM) i7-4790 CPU** produced the following results:

```
perf stat -e LLC-loads,LLC-stores,L1-dcache-loads,L1-dcache-stores ./bench 0
core: sum: 268435456, time: 5.185835

 Performance counter stats for './bench 0':

            33.762      LLC-loads                                                   
       268.214.140      LLC-stores                                                  
       270.813.206      L1-dcache-loads                                             
       270.280.665      L1-dcache-stores                                            

       5,186131658 seconds time elapsed

perf stat -e LLC-loads,LLC-stores,L1-dcache-loads,L1-dcache-stores ./bench 1
smt: sum: 268435456, time: 1.386351

 Performance counter stats for './bench 1':

            11.749      LLC-loads                                                   
             2.872      LLC-stores                                                  
       269.437.743      L1-dcache-loads                                             
       269.026.262      L1-dcache-stores                                            

       1,386712135 seconds time elapsed
```

- Each LLC store is also a store to L1.
- Using smt, ``sync_add_and_fetch()`` only results in L1 stores, while different cores have to communicate via shared L3.
- Note that `LLC-loads` for core synchronization cannot be valid, since each core has to load the value from L3 before it can be updated.
