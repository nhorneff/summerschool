#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# make sure there's no preset variables
unset SLURM_MEM_PER_NODE
unset SLURM_MEM_PER_GPU
unset SLURM_MEM_PER_CPU

# Run the program
srun ./code/prog.x
