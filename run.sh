
echo "Running stencil with 64 cores and different iterations..."


for ITERATIONS in 100 500 1000
do
    for SIZE in 1024 2048 4096 8192 16384 30720
    do
        echo "AXPY $SIZE with iterations: $ITERATIONS"
        rm -rf logs/axpy_${ITERATIONS}.out
        TT_METAL_DEVICE_PROFILER=1 ./tt-metal_axpy_64cores/build/stencil $ITERATIONS $SIZE $SIZE >> logs/axpy_${ITERATIONS}_${SIZE}.out
        python3 extract_ker_times.py --iterations $ITERATIONS >> logs/axpy_${ITERATIONS}_${SIZE}.out
    done
done

# 100 500 1000
# 1024 2048 4096
for ITERATIONS in 1000
do
    for SIZE in 4096
    do
        echo "MATMUL $SIZE with iterations: $ITERATIONS"
        rm -rf logs/matmul_${ITERATIONS}.out
        TT_METAL_DEVICE_PROFILER=1 ./tt-metal_matmul_64cores/build/stencil $ITERATIONS $SIZE $SIZE >> logs/matmul_${ITERATIONS}_${SIZE}.out
        python3 extract_ker_times.py --iterations $ITERATIONS >> logs/matmul_${ITERATIONS}_${SIZE}.out
    done
done


python3 parse_logs.py

echo "Done."