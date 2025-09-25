import numpy as np
from scipy.ndimage import laplace

# Load input matrix (assuming saved as CSV)
matrix = np.loadtxt("inputs/basic_input.csv")

# Compute Laplacian using SciPy
laplacian_matrix = laplace(matrix, mode="constant")

print(matrix)
print(laplacian_matrix)

np.savetxt("expected_output.csv", laplacian_matrix, delimiter=" ")