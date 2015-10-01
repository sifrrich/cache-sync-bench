# cache-sync-bench
Comparison of synchronisation via L1 or L3 cache. Two threads are created and assigned to different logical proccessors, which are either located on a single core (L1 sync) or different cores, connected via L3.

To find out, which logical CPUs are physically located on the same core, refer to

`awk '/^domain/ { print $1, $2; } /^cpu/ { print $1; }' /proc/schedsta`

or

`cat /proc/cpuinfo|egrep 'processor|physical id|core id' |sed 's/^processor/\nprocessor/g'`

and set these numbers in `benc.c` using both makros `SMT0|1, CORE0|1`.
