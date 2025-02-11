import pandas as pd
import matplotlib.pyplot as plt
import os
import seaborn as sns
import numpy as np

# Read and parse data
with open('timings_output.txt', 'r') as file:
    lines = file.readlines()

# Parse data into a list of dictionaries
data = []
for line in lines:
    parts = line.strip().split(', ')
    size = int(parts[0].split(': ')[1])
    length = int(parts[1].split(': ')[1])
    time_custom = float(parts[2].split(': ')[1])
    time_gmp = float(parts[3].split(': ')[1])
    data.append({
        'size': size,
        'len': length,
        'time_custom': time_custom,
        'time_gmp': time_gmp
    })

# Convert to DataFrame
df = pd.DataFrame(data)

# Create plots directory
os.makedirs('plots', exist_ok=True)

# Set global plot style
sns.set(style='darkgrid', palette='tab10')

# Helper function to save figures
def savefig(name):
    plt.savefig(os.path.join('plots', name), bbox_inches='tight')
    plt.close()

# Plot 1: Time vs Length for each size (log scale)
sizes = sorted(df['size'].unique())
fig, axs = plt.subplots(2, 3, figsize=(18, 12))
for ax, size in zip(axs.flatten(), sizes):
    subset = df[df['size'] == size]
    ax.plot(subset['len'], subset['time_custom'], label='Custom', marker='o')
    ax.plot(subset['len'], subset['time_gmp'], label='GMP', marker='s')
    ax.set_title(f'Size {size}')
    ax.set_xlabel('Length')
    ax.set_ylabel('Time (log)')
    ax.set_yscale('log')
    ax.legend()
fig.suptitle('Time Comparison by Length for Different Sizes (Log Scale)', y=1.02)
savefig('time_vs_length_per_size_log.png')

# Plot 2: Time vs Size for each length (log scale)
lengths = sorted(df['len'].unique())
fig, axs = plt.subplots(2, 3, figsize=(18, 12))
for ax, length in zip(axs.flatten(), lengths):
    subset = df[df['len'] == length]
    ax.plot(subset['size'], subset['time_custom'], label='Custom', marker='o')
    ax.plot(subset['size'], subset['time_gmp'], label='GMP', marker='s')
    ax.set_title(f'Length {length}')
    ax.set_xlabel('Size')
    ax.set_ylabel('Time (log)')
    ax.set_yscale('log')
    ax.legend()
fig.suptitle('Time Comparison by Size for Different Lengths (Log Scale)', y=1.02)
savefig('time_vs_size_per_length_log.png')


# Plot 3: Direct algorithm comparison
plt.figure(figsize=(10, 8))
plt.loglog(df['time_gmp'], df['time_custom'], 'o', alpha=0.6)
plt.plot([1e3, 1e10], [1e3, 1e10], '--', color='gray')
plt.xlabel('GMP Time')
plt.ylabel('Custom Time')
plt.title('Direct Algorithm Comparison (Log-Log)')
savefig('direct_comparison_loglog.png')

# Plot 4: Time growth analysis
fig, axs = plt.subplots(1, 2, figsize=(18, 7))
for ax, algo in zip(axs, ['custom', 'gmp']):
    for length in lengths:
        subset = df[df['len'] == length]
        ax.plot(subset['size'], subset[f'time_{algo}'], label=f'Len {length}', marker='o')
    ax.set_xlabel('Size')
    ax.set_ylabel('Time (log)')
    ax.set_yscale('log')
    ax.set_title(f'{algo.title()} Time Growth')
    ax.legend()
savefig('time_growth_comparison.png')

print("All plots saved to 'plots' directory")