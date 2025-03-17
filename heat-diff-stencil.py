import numpy as np
import matplotlib.pyplot as plt
import time

# Parametri della simulazione
nx, ny = 21, 21  # Dimensioni della griglia
lx, ly = 1.0, 1.0  # Lunghezza dominio
Tmax = 0.0005        # Tempo massimo di simulazione

dissipation = 0.1  # Coefficiente di dissipazione del calore

# Parametri fisici e numerici
dx = lx / (nx - 1)
dy = ly / (ny - 1)
dt = 0.0001  # Passo temporale
alpha = 0.1  # Coefficiente di diffusione termica

# Condizione di stabilità CFL (Courant-Friedrichs-Lewy)
CFL = alpha * dt / dx**2
print(dx**2)
if CFL > 0.25:
    raise ValueError("Instabilità numerica: ridurre dt o aumentare dx.")

# Inizializzazione della griglia
temp = np.zeros((nx, ny))

# Condizione iniziale: picco di temperatura al centro
temp[nx//2, ny//2] = 100.0

def update_temperature(temp):
    """ Calcola il prossimo stato della temperatura con uno stencil 5-points."""
    temp_new = np.copy(temp)
    for i in range(1, nx - 1):
        for j in range(1, ny - 1):
            temp_new[i, j] += dissipation * (temp[i + 1, j] + temp[i - 1, j] + temp[i, j + 1] + temp[i, j - 1] - 4 * temp[i, j])
    return temp_new

# Simulazione
start_time = time.time()
frames = []  # Per visualizzare l'evoluzione della temperatura

splitter = 0
time_elapsed = 0.0
while time_elapsed < Tmax:
    temp = update_temperature(temp)
    time_elapsed += dt
    frames.append(np.copy(temp))
    '''
    if splitter == 0 or splitter % 175 == 0:
        frames.append(np.copy(temp))  # Salva alcuni frame per visualizzazione
    splitter +=1'
    '''

print(f"Simulazione completata in {time.time() - start_time:.2f} secondi")
print(temp)



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
