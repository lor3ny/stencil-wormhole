import re
import matplotlib.pyplot as plt
import numpy as np

import numpy as np
import matplotlib.pyplot as plt

# === PLOT 1: Compare AXPY vs MATMUL (sum of memcpy+cpu+wormhole) ===
def plot_axpy_vs_matmul(data, PALETTE):
    sum_components = [mc + c + w for mc, c, w in zip(data["memcpy"], data["cpu"], data["wormhole"])]

    # Split into AXPY and MATMUL groups
    axpy_labels = [l for l in data["labels"] if "axpy" in l]
    matmul_labels = [l for l in data["labels"] if "matmul" in l]

    axpy_values = [sum_components[i] for i, l in enumerate(data["labels"]) if "axpy" in l]
    matmul_values = [sum_components[i] for i, l in enumerate(data["labels"]) if "matmul" in l]

    x = np.arange(len(axpy_labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 6))
    ax.bar(x - width/2, axpy_values, width, label="AXPY", color=PALETTE["AXPY"])
    ax.bar(x + width/2, matmul_values, width, label="MATMUL", color=PALETTE["MATMUL"])

    ax.set_ylabel("Execution Time (s)")
    ax.set_xlabel("Jacobi Iterations")
    ax.set_title("AXPY vs MATMUL")
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels([l.split("_")[1] for l in axpy_labels])  # sizes only
    ax.legend()

    plt.tight_layout()
    plt.savefig("logs/AXPY_vs_MATMUL.png", dpi=300)
    plt.close(fig)
    print("Bar plot saved to AXPY_vs_MATMUL.png")


def plot_stacked_axpy_matmul_combined(data, PALETTE):
    def get_values(prefix, values):
        return [v for l, v in zip(data["labels"], values) if prefix in l]

    sizes = ["100", "500", "1000"]

    # Extract subsets
    axpy_memcpy = get_values("axpy", data["memcpy"])
    axpy_cpu = get_values("axpy", data["cpu"])
    axpy_wormhole = get_values("axpy", data["wormhole"])

    matmul_memcpy = get_values("matmul", data["memcpy"])
    matmul_cpu = get_values("matmul", data["cpu"])
    matmul_wormhole = get_values("matmul", data["wormhole"])

    x = np.arange(len(sizes))
    width = 0.5

    # Create a single figure with two vertical subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

    # --- AXPY subplot ---
    ax1.bar(x, axpy_memcpy, width, label="Memcpy", color=PALETTE["Memcpy"])
    ax1.bar(x, axpy_cpu, width, bottom=np.array(axpy_memcpy), label="CPU", color=PALETTE["CPU"])
    ax1.bar(x, axpy_wormhole, width, bottom=np.array(axpy_memcpy) + np.array(axpy_cpu),
            label="Wormhole", color=PALETTE["Wormhole"])

    ax1.set_ylabel("Execution Time (s)")
    ax1.set_title("AXPY Execution Breakdown")
    ax1.legend()

    # --- MATMUL subplot ---
    ax2.bar(x, matmul_memcpy, width, label="Memcpy", color=PALETTE["Memcpy"])
    ax2.bar(x, matmul_cpu, width, bottom=np.array(matmul_memcpy), label="CPU", color=PALETTE["CPU"])
    ax2.bar(x, matmul_wormhole, width, bottom=np.array(matmul_memcpy) + np.array(matmul_cpu),
            label="Wormhole", color=PALETTE["Wormhole"])

    ax2.set_ylabel("Execution Time (s)")
    ax2.set_title("MATMUL Execution Breakdown")
    ax2.set_xticks(x)
    ax2.set_xticklabels(sizes)
    ax2.legend()

    plt.xlabel("Problem Size")
    plt.tight_layout()
    plt.savefig("logs/AXPY_MATMUL_Breakdown.png", dpi=300)
    plt.close(fig)

    print("Combined stacked plot saved to AXPY_MATMUL_Breakdown.png")


# === PLOT 3: AXPY stacked vs CPU_BASELINE ===
def plot_axpy_vs_baseline(data, PALETTE):
    axpy_indices = [i for i, l in enumerate(data["labels"]) if "axpy" in l]
    axpy_labels = [data["labels"][i] for i in axpy_indices]

    axpy_memcpy = [data["memcpy"][i] for i in axpy_indices]
    axpy_cpu = [data["cpu"][i] for i in axpy_indices]
    axpy_wormhole = [data["wormhole"][i] for i in axpy_indices]
    axpy_baseline = [data["cpu_baseline"][i] for i in axpy_indices]

    x = np.arange(len(axpy_labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 6))

    # Stacked AXPY
    ax.bar(x - width/2, axpy_memcpy, width, label="Memcpy", color=PALETTE["Memcpy"])
    ax.bar(x - width/2, axpy_cpu, width, bottom=np.array(axpy_memcpy), label="CPU", color=PALETTE["CPU"])
    ax.bar(x - width/2, axpy_wormhole, width, bottom=np.array(axpy_memcpy) + np.array(axpy_cpu), label="Wormhole", color=PALETTE["Wormhole"])

    # Baseline bars
    ax.bar(x + width/2, axpy_baseline, width, color=PALETTE["Baseline"], label="CPU Baseline")

    ax.set_ylabel("Execution Time (s)")
    ax.set_title("AXPY Latency Breakdown vs CPU_BASELINE")
    ax.set_xticks(x)
    ax.set_xticklabels([lbl.replace("axpy_", "") for lbl in axpy_labels])
    ax.legend()

    plt.tight_layout()
    plt.savefig("logs/AXPY_vs_BASELINE.png", dpi=300)
    plt.close(fig)

    print("Stacked bar plot saved to AXPY_vs_BASELINE.png")



def plot_ker_it_vs_wormhole(data, PALETTE):
    """
    Plot a comparison between KER_IT and WORMHOLE execution times.
    
    Args:
        data (dict): Dictionary containing 'labels', 'ker_it', and 'wormhole'.
        PALETTE (dict): Dictionary with color definitions.
    """
    labels = data["labels"]
    ker_it_times = data["ker_it"]
    wormhole_times = data["wormhole"]
    
    x = np.arange(len(labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Bars
    ax.bar(x - width/2, ker_it_times, width, label="Tracy-measured Kernel", color=PALETTE.get("AXPY", "#19E053"))
    ax.bar(x + width/2, wormhole_times, width, label="Host-measured Kernel", color=PALETTE.get("Wormhole", "#7D5CF2"))
    
    # Labels and formatting
    ax.set_ylabel("Execution Time (s)")
    ax.set_title("KER_IT vs WORMHOLE Execution Time Comparison")
    ax.set_xticks(x)
    ax.set_xticklabels([lbl.replace("axpy_", "") for lbl in labels])
    ax.legend()
    
    plt.tight_layout()
    plt.savefig("logs/KER_IT_vs_WORMHOLE.png", dpi=300)
    plt.close(fig)
    
    print("KER_IT vs WORMHOLE comparison plot saved to KER_IT_vs_WORMHOLE.png")


def plot_ker_it_vs_cpu(data, PALETTE):
    """
    Plot a comparison between KER_IT and WORMHOLE execution times.
    
    Args:
        data (dict): Dictionary containing 'labels', 'ker_it', and 'wormhole'.
        PALETTE (dict): Dictionary with color definitions.
    """
    labels = data["labels"]
    ker_it_times = data["ker_it"]
    wormhole_times = data["cpu_baseline"]
    
    x = np.arange(len(labels))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Bars
    ax.bar(x - width/2, ker_it_times, width, label="Tracy-measured Kernel", color=PALETTE.get("AXPY", "#19E053"))
    ax.bar(x + width/2, wormhole_times, width, label="CPU Baseline", color=PALETTE.get("Baseline", "#7D5CF2"))
    
    # Labels and formatting
    ax.set_ylabel("Execution Time (s)")
    ax.set_title("Wormhole Kernel vs CPU")
    ax.set_xticks(x)
    ax.set_xticklabels([lbl.replace("axpy_", "") for lbl in labels])
    ax.legend()
    
    plt.tight_layout()
    plt.savefig("logs/KER_IT_vs_CPU.png", dpi=300)
    plt.close(fig)
    
    print("KER_IT vs WORMHOLE comparison plot saved to KER_IT_vs_CPU.png")


def parse_logs(data, log_files):

    # Regex patterns to extract values
    patterns = {
        "TOTAL": re.compile(r"-TOTAL-\s+([\d.]+)\s+ms"),
        "KER_IT": re.compile(r"-KER_IT-.*?([\d.]+)\s+ms"),
        "MEMCPY": re.compile(r"-MEMCPY-.*?([\d.]+)\s+ms"),
        "WORMHOLE": re.compile(r"-WORMHOLE-.*?([\d.]+)\s+ms"),
        "CPU": re.compile(r"-CPU-.*?([\d.]+)\s+ms"),
        "CPU_BASELINE": re.compile(r"-CPU_BASELINE-.*?([\d.]+)\s+ms")
    }

    for log in log_files:
        with open(f"logs/{log}", "r") as f:
            text = f.read()

            # Extract values (default 0.0 if not found)
            values = {
                key: (float(pattern.search(text).group(1)) / 1000.0) if pattern.search(text) else 0.0
                for key, pattern in patterns.items()
            }

        # Append values to data dictionary
        data["labels"].append(log.replace(".out", ""))
        data["iterations"].append(int(log.split("_")[1]))
        data["input size"].append(int(log.split("_")[2].replace(".out", "")))
        data["totals"].append(values["TOTAL"])
        data["ker_it"].append(values["KER_IT"])
        data["memcpy"].append(values["MEMCPY"])
        data["wormhole"].append(values["WORMHOLE"])
        data["cpu"].append(values["CPU"])
        data["cpu_baseline"].append(values["CPU_BASELINE"])
        data["other"].append(values["TOTAL"] - (values["KER_IT"] + values["MEMCPY"] + values["CPU"] + values["WORMHOLE"]))


def CleanData(data):
    for key in data.keys():
        data[key] = []
    return data


if __name__ == "__main__":

    # Darker cyberpunk palette (better on white background)
    PALETTE = {
        "Memcpy": "#F25050",      
        "CPU": "#F2CB05",        
        "Wormhole": "#7D5CF2",   
        "AXPY": "#19E053",         
        "MATMUL": "#19CAF5",      
        "Baseline": "#182417"  
    }

    # Apply a global style for larger fonts
    plt.rcParams.update({
        "font.size": 18,
        "axes.titlesize": 20,
        "axes.labelsize": 18,
        "xtick.labelsize": 16,
        "ytick.labelsize": 16,
        "legend.fontsize": 16
    })


    log_files = [
        "axpy_100_128.out", "axpy_500_128.out", "axpy_1000_128.out", "axpy_100_1024.out", "axpy_500_1024.out", "axpy_1000_1024.out",
        "matmul_100_128.out", "matmul_500_128.out", "matmul_1000_128.out", "matmul_100_1024.out", "matmul_500_1024.out", "matmul_1000_1024.out"
    ]
    data = {
        "labels": [],
        "iterations": [],
        "input size": [],
        "totals": [],
        "ker_it": [],
        "memcpy": [],
        "wormhole": [],
        "cpu": [],
        "cpu_baseline": [],
        "other": []
    }

    log_files = [
        "axpy_100_128.out", "axpy_500_128.out", "axpy_1000_128.out", "axpy_100_1024.out", "axpy_500_1024.out", "axpy_1000_1024.out",
        "matmul_100_128.out", "matmul_500_128.out", "matmul_1000_128.out", "matmul_100_1024.out", "matmul_500_1024.out", "matmul_1000_1024.out"
    ]
    parse_logs(data, log_files)
    plot_axpy_vs_matmul(data, PALETTE)
    plot_ker_it_vs_wormhole(data, PALETTE)
    CleanData(data)

    log_files = [
        "axpy_100_1024.out", "axpy_500_1024.out", "axpy_1000_1024.out",
        "matmul_100_1024.out", "matmul_500_1024.out", "matmul_1000_1024.out"
    ]
    parse_logs(data, log_files)
    plot_axpy_vs_baseline(data, PALETTE)
    plot_stacked_axpy_matmul_combined(data, PALETTE)
    CleanData(data)

    log_files = [
        "axpy_500_1024.out", "matmul_500_1024.out",
        "axpy_1000_1024.out",
        "matmul_1000_1024.out"
    ]
    parse_logs(data, log_files)
    plot_ker_it_vs_cpu(data, PALETTE)
    

    

