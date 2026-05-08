import pandas as pd
import matplotlib.pyplot as plt
import os

data = []
labels = []

# 1. Roulette data
if os.path.exists('experiment_results_roulette.csv'):
    df_roulette = pd.read_csv('experiment_results_roulette.csv')
    data.append(df_roulette['BestObjective'])
    labels.append('Roulette')

# 2. Tourneum data
if os.path.exists('experiment_results_tournament.csv'):
    df_tournament = pd.read_csv('experiment_results_tournament.csv')
    data.append(df_tournament['BestObjective'])
    labels.append('Tournament')

# 3. Generate the plot if there is data
if data:
    plt.figure(figsize = (8,6))

    box = plt.boxplot(data, patch_artist = True, labels = labels,
                      boxprops = dict(facecolor = 'lightblue', color = 'blue'),
                      medianprops = dict(color = 'red', linewidth = 2),
                      flierprops = dict(marker = 'o', color = 'red', alpha = 0.5))

    plt.title('Comparison of Selection Methods (100 runs)', fontsize = 15)
    plt.ylabel('Objective Function Value', fontsize = 12)
    plt.grid(True, linestyle = '--', alpha = 0.7)
    plt.tight_layout()

    # Save image
    plt.savefig('boxplot_comparison.png', dpi = 300)
    print("boxplot_comparison.png saved")
else:
    print("Error: No results file found")