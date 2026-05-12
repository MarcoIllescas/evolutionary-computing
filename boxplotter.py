import pandas as pd
import matplotlib.pyplot as plt
import os

file_roulette = "experiment_results_roulette.csv"
file_tournament = "experiment_results_tournament.csv"

data = []
labels = []

if os.path.exists(file_roulette) and os.path.exists(file_tournament):
    df_roulette = pd.read_csv(file_roulette)
    data.append(df_roulette['BestObjective'])
    labels.append("Roulette")

    df_tournament = pd.read_csv(file_tournament)
    data.append(df_tournament['BestObjective'])
    labels.append("Tournament")

    plt.figure(figsize = (8,6))

    box = plt.boxplot(data, patch_artist = True, tick_labels = labels,
                      boxprops = dict(facecolor = 'lightblue', color = 'blue'),
                      medianprops = dict(color = 'red', linewidth = 2),
                      flierprops = dict(marker = 'o', color = 'red', alpha = 0.5))

    plt.title("Selection Method Comparison (100 runs)", fontsize = 15)
    plt.ylabel("Objective Function Value (Maximized)", fontsize = 12)
    plt.grid(True, linestyle = '--', alpha = 0.7)
    plt.tight_layout()

    plt.savefig('boxplot_comparison.png', dpi = 300)
    print("Boxplot comparison plot saved at 'boxplot_comparison.png'")

else:
    print("=======================================================================")
    print("Warning Boxplot: To generate the plot it is necessary to have both .csv")
    if not os.path.exists(file_roulette):
        print(" -> Roulette results missing.")
    if not os.path.exists(file_tournament):
        print(" -> Tournament results missing.")
    print("Plot will generate when both csv files are present.")
    print("=======================================================================")