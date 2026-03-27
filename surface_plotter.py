import sys
import numpy as np
import matplotlib.pyplot as plt

# 1. Receive data from the best individual from C++
try:
    best_x = float(sys.argv[1])
    best_y = float(sys.argv[2])
    best_z = float(sys.argv[3])
except IndexError:
    print("Error: X, Y, and Z coordinates were expected as arguments.")
    sys.exit(1)

# 2. Create math grid
x = np.linspace(-6.28, 6.28, 200)
y = np.linspace(-6.28, 6.28, 200)
X, Y = np.meshgrid(x, y)

# 3. Calculate elevation (Z) with the equation
exponent = -((X + 1)**2 + (Y - 3.14)**2) / 25.0
Z = 10 * np.exp(exponent) + np.cos(2 * X) + np.sin(2 * Y)

# 4. Configure 3D canvas
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# 5. Draw mathematical surface
superficie = ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.8, edgecolor='none')
fig.colorbar(superficie, ax=ax, shrink=0.5, aspect=10, label='Elevation (Z)')

# 6. Mark the optimal point found by the Genetic Algorithm
ax.scatter([best_x], [best_y], [best_z], color='red', s=200, marker='*', label=f'Óptimo GA: {best_z:.3f}')

# Plot style
ax.set_title('Objective Function Surface and Global Optimum', fontsize=16)
ax.set_xlabel('Axis X')
ax.set_ylabel('Axis Y')
ax.set_zlabel('Objective Value (Z)')
ax.legend()

# 7. Guardar y mostrar
plt.savefig('surface_plot.png', dpi=300)
print("3D graph successfully generated as 'surface_plot.png'!")