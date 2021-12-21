all: 
	./bin/clang++ -std=c++17 -O2 -fopencilk -DUSE_CILK_PLUS_RUNTIME bfs_start.cpp -I pctl/include -I cmdline/include -I chunkedseq/include -o program

run: all
	CILK_NWORKERS=4 ./program