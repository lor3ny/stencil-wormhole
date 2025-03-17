import numpy as np
import matplotlib.pyplot as plt


# Lettura della matrice da un file .csv
def read_matrix_from_csv(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        ncols, nrows = map(int, lines[0].strip().split())
        matrix = np.zeros((nrows, ncols))
        for i, line in enumerate(lines[1:]):
            matrix[i] = np.array(list(map(float, line.split())))
    return matrix

if __name__ == '__main__':
    file_path = 'result.csv'
    frames = []
    frames.append(read_matrix_from_csv(file_path))
    lx, ly = frames[0].shape
    Tmax = 0.2

    # Visualizzazione
    ncols = 4
    nrows = (len(frames) + ncols - 1) // ncols  # Determina il numero di righe necessarie
    fig, axes = plt.subplots(nrows, ncols, figsize=(15, 3 * nrows))
    axes = axes.flatten()
    for i, frame in enumerate(frames):
        axes[i].imshow(frame, cmap='hot', origin='lower', extent=[0, lx, 0, ly])
        axes[i].set_title(f"t = {i * Tmax / len(frames):.3f}s")
        axes[i].axis('off')

    # Nasconde gli assi extra
    for i in range(len(frames), len(axes)):
        axes[i].axis('off')

    plt.show()
