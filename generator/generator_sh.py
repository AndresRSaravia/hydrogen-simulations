intitles = [
	'v0c00s', 'v0c00c',
	'v1c00s', 'v1c00c', 'v1c01s', 'v1c01c',
	'v2c00s', 'v2c00c', 'v2c01s', 'v2c01c', 'v2c02s', 'v2c02c',
	'v2c03s', 'v2c03c', 'v2c04s', 'v2c04c', 'v2c05s', 'v2c05c',
	'v3c00s', 'v3c00c', 'v3c01s', 'v3c01c', 'v3c02s', 'v3c02c',
	'v3c03s', 'v3c03c', 'v3c04s', 'v3c04c', 'v3c05s', 'v3c05c',
	'v3c06s', 'v3c06c', 'v3c07s', 'v3c07c', 'v3c08s', 'v3c08c',
	'v3c09s', 'v3c09c',
	'v4c00s', 'v4c00c', 'v4c01s', 'v4c01c', 'v4c02s', 'v4c02c',
	'v4c03s', 'v4c03c', 'v4c04s', 'v4c04c', 'v4c05s', 'v4c05c',
	'v4c06s', 'v4c06c', 'v4c07s', 'v4c07c', 'v4c08s', 'v4c08c',
	'v4c09s', 'v4c09c', 'v4c10s', 'v4c10c', 'v4c11s', 'v4c11c',
	'v4c12s', 'v4c12c',
	'v5c00s', 'v5c00c', 'v5c01s', 'v5c01c', 'v5c02s', 'v5c02c',
	'v5c03s', 'v5c03c', 'v5c04s', 'v5c04c', 'v5c05s', 'v5c05c',
	'v5c06s', 'v5c06c', 'v5c07s', 'v5c07c', 'v5c08s', 'v5c08c',
	'v5c09s', 'v5c09c', 'v5c10s', 'v5c10c',
	'v6c00s', 'v6c00c', 'v6c01s', 'v6c01c', 'v6c02s', 'v6c02c',
	'v6c03s', 'v6c03c', 'v6c04s', 'v6c04c', 'v6c05s', 'v6c05c',
	'v7c00s', 'v7c00c', 'v7c01s', 'v7c01c', 'v8c00s', 'v8c00c'
]

for intitle in intitles:
	shcontent = f"""#!/bin/bash

### Serafin - Cola multi

### Nombre de la tarea
#SBATCH --job-name={intitle}

### Cola a usar
#SBATCH --partition=multi

### Cantidad de nodos a usar
### mono/gpu: 1, multi: 2-8
#SBATCH --nodes=1

### Cores a utilizar por nodo = procesos por nodo * cores por proceso
### Cantidad de procesos a lanzar por nodo
#SBATCH --ntasks-per-node=64

### Cores por proceso (para MPI+OpenMP)
#SBATCH --cpus-per-task=1

### Tiempo de ejecucion. Formato dias-horas:minutos.
### multi: <= 2 dias, short: <= 1 hora
#SBATCH --time 1-20:00

#---------------------------------------------------
# Script que se ejecuta al arrancar el trabajo

# Cargar el entorno del usuario incluyendo la funcionalidad de modules
# No tocar
. /etc/profile

# Configurar OpenMP y otras bibliotecas que usan threads
# usando los valores especificados arriba

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
export MKL_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Cargar los modulos para las tareas
module load quantum-espresso/7.2

# Lanzar el programa
srun pw.x -inp {intitle}.in > {intitle}.out
""".format(intitle)
	print(shcontent)
	shfile = open("../inshoutdata/insh/"+intitle+".sh","w")
	shfile.write(shcontent)
	shfile.close()

"""
for i in range(4):
	temp_str = ""
	for intitle in intitles[i*26:(i+1)*26]:
		temp_str += f""" #sbatch {intitle}.sh
""".format(intitle)
	shallfile = open("../inshdata/runall"+str(i)+".sh","w")
	shallfile.write(temp_str)
	shallfile.close()
"""
