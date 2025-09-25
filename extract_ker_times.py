import pandas as pd
import numpy as np
import argparse
import matplotlib.pyplot as plt

def analyze_and_plot_kernels(csv_file, kernel_zones, output_file="timeline.png"):
    """
    Analyze multiple kernel zones and plot timeline per core.
    
    kernel_zones: list of zone names, e.g. ["WRITER KERNEL", "STENCIL KERNEL", "READER KERNEL"]
    """
    print(f"Analyzing zones: {kernel_zones}")

    df = pd.read_csv(csv_file, skiprows=1)
    
    execution_cycles = {}

    # Assign colors per kernel
    kernel_colors = {
        "WRITER KERNEL": "tab:blue",
        "STENCIL KERNEL": "tab:orange",
        "READER KERNEL": "tab:green"
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
                    "color": kernel_colors.get(kernel_name, "gray")
                })

    # ---- Plot ----
    fig, ax = plt.subplots(figsize=(15, 40))
    
    cores = list(sorted(set(d["core"] for d in timeline_data)))
    y_positions = {core: i for i, core in enumerate(cores)}

    # Plot bars
    for d in timeline_data:
        ax.barh(y_positions[d["core"]],
                d["duration"],
                left=d["start"],
                height=0.4,
                color=d["color"],
                align="center",
                label=d["kernel"])

    ax.set_yticks(list(y_positions.values()))
    ax.set_yticklabels(list(y_positions.keys()))
    ax.set_xlabel("Cycles since reset")
    ax.set_ylabel("Core + Processor")
    ax.set_title("Kernel Execution Timeline per Core")

    # Avoid duplicate labels in legend
    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(by_label.values(), by_label.keys(), title="Kernel Type")

    plt.tight_layout()
    plt.savefig(output_file, dpi=300)
    plt.close(fig)

    print(f"Timeline saved to {output_file}")
    return timeline_data



# Load CSV
def analyze_execution_cycles(csv_file, zone_name):

    print(f"Analyzing zone: {zone_name}")

    # Read CSV
    df = pd.read_csv(csv_file, skiprows=1)
    
    # Filter ALL_RED_LOOP
    all_red_loop = df[df['  zone name'] == zone_name]
    
    # Group by core_x, core_y, RISC processor type, and zone phase
    grouped = all_red_loop.groupby([' core_x', ' core_y', ' RISC processor type', ' type'])
    
    # Extract latest begin and end for each core_x, core_y, and processor type
    execution_cycles = {}
    for (core_x, core_y, processor, phase), group in grouped:
        latest_entry = group.sort_values(' time[cycles since reset]', ascending=False).iloc[0]
        key = (core_x, core_y)
        if key not in execution_cycles:
            execution_cycles[key] = {'ZONE_START': {}, 'ZONE_END': {}}
        if phase not in execution_cycles[key]:
            execution_cycles[key][phase] = {}
        execution_cycles[key][phase][processor] = latest_entry[' time[cycles since reset]']

    # Calculate execution times and store with core information
    execution_times = []
    core_times = []  # List to store tuples of (execution_time, core_x, core_y)
    for (core_x, core_y), phases in execution_cycles.items():
        if 'ZONE_START' in phases and 'ZONE_END' in phases:
            latest_begin = max(phases['ZONE_START'].values())
            latest_end = max(phases['ZONE_END'].values())
            exec_time = latest_end - latest_begin
            execution_times.append(exec_time)
            core_times.append((exec_time, core_x, core_y))
    
    # Compute statistics
    execution_times = np.array(execution_times)
    min_time = np.min(execution_times)
    lower_quartile = np.percentile(execution_times, 25)
    mean = np.mean(execution_times)
    median = np.median(execution_times)
    upper_quartile = np.percentile(execution_times, 75)
    max_time = np.max(execution_times)
    
    # Find cores with min and max times
    min_core = next(ct for ct in core_times if ct[0] == min_time)
    max_core = next(ct for ct in core_times if ct[0] == max_time)
    
    # Print results
    print(f"Min: {min_time} nanoseconds {min_time/1000000} ms (Core {min_core[1]},{min_core[2]})")
    print(f"Lower Quartile: {lower_quartile} nanoseconds {lower_quartile/1000000} ms")
    print(f"Mean: {mean} nanoseconds {mean/1000000} ms")
    print(f"Median: {median} nanoseconds {median/1000000} ms")
    print(f"Upper Quartile: {upper_quartile} nanoseconds {upper_quartile/1000000} ms")
    print(f"Max: {max_time} nanoseconds {max_time/1000000} ms (Core {max_core[1]},{max_core[2]})\n")


def compute_overall_duration(csv_file, kernels, iterations):
    """
    Computes the overall execution duration across multiple kernels.
    
    Returns:
        overall_start: earliest ZONE_START among all kernels
        overall_end: latest ZONE_END among all kernels
        duration: overall_end - overall_start
    """
    df = pd.read_csv(csv_file, skiprows=1)
    
    all_starts = []
    all_ends = []
    
    for kernel_name in kernels:
        zone_df = df[df['  zone name'] == kernel_name]
        grouped = zone_df.groupby([' core_x', ' core_y', ' RISC processor type', ' type'])
        for (core_x, core_y, processor, phase), group in grouped:
            latest_entry = group.sort_values(' time[cycles since reset]', ascending=False).iloc[0]
            if phase == "ZONE_START":
                all_starts.append(latest_entry[' time[cycles since reset]'])
            elif phase == "ZONE_END":
                all_ends.append(latest_entry[' time[cycles since reset]'])

    if not all_starts or not all_ends:
        print("No start or end times found for the given kernels.")
        return None, None, None

    overall_start = min(all_starts)
    overall_end = max(all_ends)
    duration = overall_end - overall_start

    print(f"Overall start: {overall_start}")
    print(f"Overall end: {overall_end}")
    print(f"-KER- {duration} nanoseconds {duration/1000000} ms")
    print(f"-KER_IT- {duration*iterations} nanoseconds {(duration*iterations)/1000000} ms")
    
    return overall_start, overall_end, duration

# Examanalyze_execution_cycles(csv_file)ple usage


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Analyze kernel execution cycles.")
    parser.add_argument("--iterations", type=int, required=True, help="Number of iterations")
    args = parser.parse_args()

    csv_file = '/home/lpiarulli_tt/tt-metal/generated/profiler/.logs/profile_log_device.csv'
    kernels = ["WRITER KERNEL", "STENCIL KERNEL", "READER KERNEL"]

    analyze_execution_cycles(csv_file, 'STENCIL KERNEL')
    analyze_execution_cycles(csv_file, 'READER KERNEL')
    analyze_execution_cycles(csv_file, 'WRITER KERNEL')

    compute_overall_duration(csv_file, kernels, args.iterations)

    #timeline = analyze_and_plot_kernels(csv_file, kernels, f"timeline_{args.iterations}.png")


#which works out 1 microsecond per cycle
#since it's 1MHz clock speed