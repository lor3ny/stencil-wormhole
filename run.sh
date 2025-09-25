
echo "Running stencil with 64 cores and different iterations..."

for ITERATIONS in 1 10 100 1000
do
    echo "MATMUL with iterations: $ITERATIONS"
    rm -rf matmul_${ITERATIONS}.out
    TT_METAL_DEVICE_PROFILER=1 ./tt-metal_matmul_64cores/build/stencil $ITERATIONS 64 64 >> matmul_${ITERATIONS}.out
    python3 /home/lpiarulli_tt/stencil-wormhole/latency_analysis.py --iterations $ITERATIONS >> matmul_${ITERATIONS}.out
done


for ITERATIONS in 1 10 100 1000
do
    echo "AXYP with iterations: $ITERATIONS"
    rm -rf axpy_${ITERATIONS}.out
    TT_METAL_DEVICE_PROFILER=1 ./tt-metal_axpy_64cores/build/stencil $ITERATIONS 64 64 >> axpy_${ITERATIONS}.out
    python3 /home/lpiarulli_tt/stencil-wormhole/latency_analysis.py --iterations $ITERATIONS >> axpy_${ITERATIONS}.out
done

echo "Done."