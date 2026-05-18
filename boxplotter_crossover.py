import pandas as pd
import matplotlib.pyplot as plt
import os

from boxplotter_tournament import missing_files

files = {
    1: ("crossover_results_1pt.csv", "Cruza de 1 Punto"),
    2: ("crossover_results_2pt.csv", "Cruza de 2 Puntos"),
    3: ("crossover_results_uniform.csv", "Cruza Uniforme")
}

data = []
labels = []
missing_files = []

for method_id, (filename, label) in files.items():
    if os.path.exists(filename):
        df = pd.read_csv(filename)
        data.append(df['BestObjective'])
        labels.append(label)
    else:
        missing_files.append(filename)

if not missing_files and data:
    plt.figure(figsize = (10, 6))

    box = plt.boxplot(data, patch_artist = True, tick_labels = labels,
                      boxprops = dict(facecolor = 'lightcoral', color = 'darkred'),
                      medianprops = dict(color = 'yellow', linewidth = 2),
                      flierprops = dict(marker = 'o', color = 'black', alpha = 0.5))

    plt.title('Cross Operators Comparison (100 runs)', fontsize = 15)
    plt.ylabel('Objective Function Value (Maximized)', fontsize = 12)
    plt.grid(True, linestyle = '--', alpha = 0.7)
    plt.tight_layout()

    plt.savefig('boxplots_crossover_comparison.png', dpi = 300)
    print("Comparative Boxplot generated successfully as 'boxplots_crossover_comparison.png'.")

else:
    print("---------------------------------------------------------------------")
    print("Boxplot progress: Cant complete without all the files")
    for missing_file in missing_files:
        print(f"Missing file: {missing_file}")
    print("Plot will generate when all files are complete.")