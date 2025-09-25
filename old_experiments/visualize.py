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

    for idx, frame in enumerate(frames):
        plt.imshow(frame, cmap='viridis', origin='lower')
        plt.colorbar()
        plt.title(f'Time: {idx * Tmax:.2f}')
        plt.savefig(f'frame_{idx}.png')
        plt.clf()

    plt.show()
