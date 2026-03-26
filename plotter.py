import pandas as pd
import matplotlib.pyplot as plt

# 1. Read data
try:
    df = pd.read_csv("ga_results.csv", skipinitialspace=True)
except FileNotFoundError:
    print("Error: ga_results.csv not found")
    exit()

# 2. Plot configuration
plt.figure(figsize=(10, 6))
plt.plot(df['Generation'], df['Best'], label='Best Individual', color='green', linewidth=2)
plt.plot(df['Generation'], df['Average'], label='Population Average', color='blue', linestyle='--')
plt.plot(df['Generation'], df['Worst'], label='Worst Individual', color='red', alpha=0.5)

# 3. Plot style
plt.title('Genetic Algorithm Evaluation', fontsize=16)
plt.xlabel('Generation', fontsize=12)
plt.ylabel('Objective Function Value', fontsize=12)
plt.legend()
plt.grid(True, linestyle=':', alpha=0.7)
plt.tight_layout()

# 4. Guardar la imagen y cerrar
plt.savefig('convergence_graph.png', dpi=300)
print("Graph successfully generated as 'convergence_graph.png'")