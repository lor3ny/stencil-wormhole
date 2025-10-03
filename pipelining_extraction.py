import pandas as pd
import numpy as np
import argparse
import matplotlib.pyplot as plt

import pandas as pd
import matplotlib.pyplot as plt

def analyze_and_plot_kernels(csv_file, kernel_zones, output_file="timeline.png"):
    """
    Analyze multiple kernel zones and plot timeline per core.
    
    kernel_zones: list of zone names, e.g. ["Writer Kernels", "Compute Kernels", "Reader Kernels"]
    """
    print(f"Analyzing zones: {kernel_zones}")

    df = pd.read_csv(csv_file, skiprows=1)
    
    execution_cycles = {}

    # Use the cyberpunk darker palette
    kernel_colors = {
        "Writer Kernels": "#F25050",   # cyberpunk red
        "Compute Kernels": "#F2CB05",  # neon yellow
        "Reader Kernels": "#7D5CF2"    # violet
    }

    # Extract start and end for each kernel zone
    for zone_name in kernel_zones:
        zone_df = df[df['  zone name'] == zone_name]
        grouped = zone_df.groupby([' core_x', ' core_y', ' RISC processor type', ' type'])
        for (core_x, core_y, processor, phase), group in grouped:
            latest_entry = group.sort_values(' time[cycles since reset]', ascending=False).iloc[0]
            key = (core_x, core_y, processor)
            if key not in execution_cycles:
                execution_cycles[key] = {}
            if zone_name not in execution_cycles[key]:
                execution_cycles[key][zone_name] = {'ZONE_START': None, 'ZONE_END': None}
            execution_cycles[key][zone_name][phase] = latest_entry[' time[cycles since reset]']

    # Collect timeline data
    timeline_data = []
    for (core_x, core_y, processor), zones in execution_cycles.items():
        core_label = f"Core({core_x},{core_y})-{processor}"
        for kernel_name, phases in zones.items():
            if phases['ZONE_START'] is not None and phases['ZONE_END'] is not None:
                start = phases['ZONE_START']
                end = phases['ZONE_END']
                timeline_data.append({
                    "core": core_label,
                    "kernel": kernel_name,
                    "start": start,
                    "duration": end - start,
                    "color": kernel_colors.get(kernel_name, "gray"),
                    "core_id": (core_x, core_y)   # keep grouping info
                })

    # ---- Normalize start times so that the first kernel begins at 0 ----
    min_start = min(d["start"] for d in timeline_data)
    for d in timeline_data:
        d["start"] -= min_start

    # ---- Assign y positions with gaps between cores ----
    cores = list(sorted(set(d["core"] for d in timeline_data)))
    core_groups = sorted(set(d["core_id"] for d in timeline_data))

    y_positions = {}
    y_index = 0
    for group in core_groups:
        group_cores = [c for c in cores if f"Core({group[0]},{group[1]})" in c]
        for c in group_cores:
            y_positions[c] = y_index
            y_index += 1
        y_index += 1  # add a blank space between different cores

    # ---- Plot ----
    fig, ax = plt.subplots(figsize=(17, 12))  # same size as other plots
    
    for d in timeline_data:
        ax.barh(y_positions[d["core"]],
                d["duration"],
                left=d["start"],
                height=0.6,
                color=d["color"],
                align="center",
                label=d["kernel"])

    # Formatting
    ax.set_yticks(list(y_positions.values()))
    ax.set_yticklabels(list(y_positions.keys()), fontsize=16)
    ax.set_xlabel("Cycles (normalized)", fontsize=24, labelpad=12)
    ax.set_ylabel("Baby Core + Thread", fontsize=24, labelpad=12)
    ax.set_title("Kernel Execution Timeline per Core", fontsize=24, pad=15)
    ax.tick_params(axis="x", labelsize=20)
    ax.tick_params(axis="y", labelsize=20)

    # Avoid duplicate labels in legend
    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(
        by_label.values(),
        by_label.keys(),
        title="Kernel Type",
        fontsize=18,
        title_fontsize=16,
        frameon=True,
        facecolor="white",
        edgecolor="black",
    )

    plt.tight_layout()
    plt.savefig("logs/"+output_file, dpi=300)
    plt.close(fig)

    print(f"Timeline saved to {output_file}")




if __name__ == "__main__":
    csv_file = '/home/lpiarulli_tt/tt-metal/generated/profiler/.logs/profile_log_device.csv'
    kernels = ["Writer Kernels", "Compute Kernels", "Reader Kernels"]
    plt.style.use("seaborn-v0_8-whitegrid")
    timeline = analyze_and_plot_kernels(csv_file, kernels, f"PIPELINING_.png")