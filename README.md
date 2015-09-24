# cache-sync-bench
Comparison of synchronisation using L1 or L3 cache

To find out, which logical CPUs are physically located on the same core, refer to
awk '/^domain/ { print $1, $2; } /^cpu/ { print $1; }' /proc/schedsta

cat /proc/cpuinfo|egrep 'processor|physical id|core id' |sed 's/^processor/\nprocessor/g'

and set these numbers in benc.c
