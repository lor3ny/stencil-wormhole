import pandas as pd
import numpy as np

# Load CSV
def analyze_execution_cycles(csv_file):
    # Read CSV
    df = pd.read_csv(csv_file, skiprows=1)
    
    # Filter ALL_RED_LOOP
    all_red_loop = df[df['  zone name'] == 'STENCIL KERNEL']
    
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
    print(f"Min: {min_time} microseconds {min_time/1000} milliseconds (Core {min_core[1]},{min_core[2]})")
    print(f"Lower Quartile: {lower_quartile} microseconds {lower_quartile/1000} milliseconds")
    print(f"Mean: {mean} microseconds {mean/1000} milliseconds")
    print(f"Median: {median} microseconds {median/1000} milliseconds")
    print(f"Upper Quartile: {upper_quartile} microseconds {upper_quartile/1000} milliseconds")
    print(f"Max: {max_time} microseconds {max_time/1000} milliseconds (Core {max_core[1]},{max_core[2]})")

# Example usage
csv_file = '/home/lpiarulli_tt/tt-metal/generated/profiler/.logs/profile_log_device.csv'
analyze_execution_cycles(csv_file)


#which works out 1 microsecond per cycle
#since it's 1MHz clock speed