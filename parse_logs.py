import re
import matplotlib.pyplot as plt
import numpy as np

# List of log files
log_files = [
    "axpy_100.out", "axpy_500.out", "axpy_1000.out",
    "matmul_100.out", "matmul_500.out", "matmul_1000.out"
]

# Regex patterns to extract values
patterns = {
    "TOTAL": re.compile(r"-TOTAL-\s+([\d.]+)\s+ms"),
    "KER_IT": re.compile(r"-KER_IT-.*?([\d.]+)\s+ms"),
    "MEMCPY": re.compile(r"-MEMCPY-.*?([\d.]+)\s+ms"),
    "WORMHOLE": re.compile(r"-WORMHOLE-.*?([\d.]+)\s+ms"),
    "CPU": re.compile(r"-CPU-.*?([\d.]+)\s+ms"),
    "CPU_BASELINE": re.compile(r"-CPU_BASELINE-.*?([\d.]+)\s+ms")
}

# Dictionary to store parsed values
results = {log: {} for log in log_files}

for log in log_files:
    with open(f"logs/{log}", "r") as f:
        text = f.read()

        for key, pattern in patterns.items():
            match = pattern.search(text)
            if match:
                results[log][key] = float(match.group(1))
            else:
                results[log][key] = 0.0  # default if not found

# Prepare data for plotting
labels = [log.replace(".out", "") for log in log_files]
totals = [results[log]["TOTAL"] for log in log_files]
ker_it = [results[log]["KER_IT"] for log in log_files]
memcpy = [results[log]["MEMCPY"] for log in log_files]
wormhole = [results[log]["WORMHOLE"] for log in log_files]
cpu = [results[log]["CPU"] for log in log_files]
cpu_baseline = [results[log]["CPU_BASELINE"] for log in log_files]

# Compute the "other" part (everything not in KER_IT, MEMCPY, CPU)
other = [t - (ki + mc + c + w) for t, ki, mc, c, w in zip(totals, ker_it, memcpy, cpu, wormhole)]

# Plot stacked bar chart
x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots(figsize=(12, 6))

ax.bar(x - width/2, ker_it, width, label="Kernel Iteration (-KER_IT-)")
ax.bar(x - width/2, memcpy, width, bottom=np.array(ker_it), label="Memcpy (-MEMCPY-)")
ax.bar(x - width/2, cpu, width, bottom=np.array(ker_it) + np.array(memcpy), label="CPU (-CPU-)")
ax.bar(x - width/2, wormhole, width, bottom=np.array(ker_it) + np.array(memcpy)+np.array(cpu), label="Wormhole (-WORMHOLE-)")
ax.bar(x - width/2, other, width, bottom=np.array(ker_it) + np.array(memcpy) + np.array(cpu) + np.array(wormhole), label="Other")

# === BASELINE ===
ax.bar(x + width/2, cpu_baseline, width, color="gray", label="CPU_BASELINE")

ax.set_yscale("log")
ax.set_ylabel("Time (ms, log scale)")
ax.set_title("Latency Breakdown from Logs")
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=45, ha="right")
ax.legend()

plt.tight_layout()
plt.savefig("logs/Latency_plot.png", dpi=300)
plt.close(fig)

print("Stacked bar plot saved to Latency_plot.png")
