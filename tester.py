
import subprocess
#import pandas as pd

command = ["mpirun", "-np", "4", "jacobi_method"]

# Vedere come i codici di saverio prendono input, in caso modificarli per parsare input da .csv

# Scrivere degli input in .csv (tipo 5)

# Leggere il file di output prodotto da mpirun

def executeMPI():
    for i in range(5):
        with open("output", "w") as outfile, open("public_results", "r") as results:
            try:
                result = subprocess.run(command, stdout=outfile,  stderr=subprocess.STDOUT, text=True)

                # PARSE OUTFILE

                # PARSE RESULTS

                # COMPARE THEM

                print(f"Test {i} DONE!") 
            except subprocess.CalledProcessError as e:
                print("Errore nell'esecuzione di mpirun:", e.stderr)



if __name__ == '__main__':
    print("TESTING")
    executeMPI()