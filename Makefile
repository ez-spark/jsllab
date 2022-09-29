.PHONY:

all:
	emcc ./src/*.c ./jsllab.c -w -s WASM=1 -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1  -s ABORTING_MALLOC=0 -s WASM_MEM_MAX=4GB -s EXPORTED_FUNCTIONS="['_malloc', '_free']" -s PTHREAD_POOL_SIZE=16 --preload-file *.bin --no-heap-copy -O3 -lm -lpthread -o jsllab_handler.js 
