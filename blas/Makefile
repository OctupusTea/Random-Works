.PHONY: clean

default: all

all: mkl openblas naive

mkl:
	gcc mkl.c -Wl,--start-group /opt/intel/mkl/lib/intel64/libmkl_intel_ilp64.a /opt/intel/mkl/lib/intel64/libmkl_sequential.a /opt/intel/mkl/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl -DMKL_ILP64 -m64 -I/opt/intel/mkl/include -o mkl

openblas:
	gcc openblas.c -I /mnt/e/Downloads/OpenBLAS-0.2.20/ -L/mnt/e/Downloads/OpenBLAS-0.2.20/ -lopenblas -o openblas

naive:
	gcc naive.c -o naive

clean:
	rm -f mkl openblas naive
