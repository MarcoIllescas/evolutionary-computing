import pandas as pd
import matplotlib.pyplot as plt
import os

sizes = [2, 4, 8, 16]
data = []
labels = []
missing_files = []

for size in sizes:
    filename = f"tournament_results_N{size}.csv"

    if os.path.exists(filename):
        df = pd.read_csv(filename)
        data.append(df['BestObjective'])
        labels.append(f"Tournament N={size}")
    else:
        missing_files.append(filename)

if not missing_files and data:
    plt.figure(figsize=(10, 6))

    box = plt.boxplot(data, patch_artist=True, tick_labels=labels,
                      boxprops=dict(facecolor='lightgreen', color='darkgreen'),
                      medianprops=dict(color='red', linewidth=2),
                      flierprops=dict(marker='o', color='red', alpha=0.5))

    plt.title('Tournament Sizes Comparison Performance',fontsize = 15)
    plt.ylabel('Objective Function Value (Maximized)', fontsize = 12)
    plt.grid(True, linestyle='--', alpha = 0.7)
    plt.tight_layout()

    plt.savefig(f"boxplot_tournament_sizes_comparison.png", dpi = 300)
    print("Boxplot tournament sizes comparison saved successfully")
else:
    print("=======================================================================")
    print("Boxplot progress: Cant complete without all the sizes.")
    for missing in missing_files:
        print(f" -> Missing files: {missing}")
    print("Plot will generate when all csv files are present.")
    print("=======================================================================")