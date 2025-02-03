
import subprocess
import pandas as pd

command = ["mpirun", "-np", "4", "mpi_script.exe", ">", "output.txt", "2>", "error.txt"]
command_win = ["mpiexec", "-np", "4", "jacobi_method.exe"]

# Vedere come i codici di saverio prendono input, in caso modificarli per parsare input da .csv

# Scrivere degli input in .csv (tipo 5)

# Leggere il file di output prodotto da mpirun

def executeMPI():


    for i in range(5):
        try:
            result = subprocess.run(command_win, check=True, capture_output=True, text=True)
            print("Output:\n", result.stdout)
        except subprocess.CalledProcessError as e:
            print("Errore nell'esecuzione di mpirun:", e.stderr)

if __name__ == '__main__':
    print("TESTING")

    executeMPI()