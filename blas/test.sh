export OPENBLAS_NUM_THREADS=1
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=2
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=3
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=4
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=5
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=6
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=7
make clean
make openblas
time ./openblas

export OPENBLAS_NUM_THREADS=8
make clean
make openblas
time ./openblas


