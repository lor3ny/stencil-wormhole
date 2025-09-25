
echo "Running stencil with 64 cores and different iterations..."

for ITERATIONS in 100 500 1000
do
    echo "MATMUL with iterations: $ITERATIONS"
    rm -rf logs/matmul_${ITERATIONS}.out
    TT_METAL_DEVICE_PROFILER=1 ./tt-metal_matmul_64cores/build/stencil $ITERATIONS 128 128 >> logs/matmul_${ITERATIONS}.out
    python3 extract_ker_times.py --iterations $ITERATIONS >> logs/matmul_${ITERATIONS}.out
done


for ITERATIONS in 100 500 1000
do
    echo "AXYP with iterations: $ITERATIONS"
    rm -rf logs/axpy_${ITERATIONS}.out
    TT_METAL_DEVICE_PROFILER=1 ./tt-metal_axpy_64cores/build/stencil $ITERATIONS 128 128 >> logs/axpy_${ITERATIONS}.out
    python3 extract_ker_times.py --iterations $ITERATIONS >> logs/axpy_${ITERATIONS}.out
done

python3 parse_logs.py

echo "Done."