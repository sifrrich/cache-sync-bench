all: run

bench: bench.c
	gcc -O3 -o $@ $< -lpthread

run: bench
	perf stat -e LLC-loads,LLC-stores,L1-dcache-loads,L1-dcache-stores ./bench 0
	perf stat -e LLC-loads,LLC-stores,L1-dcache-loads,L1-dcache-stores ./bench 1


