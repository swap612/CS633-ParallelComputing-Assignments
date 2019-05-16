./gen_machinefile.sh

echo "**Running script for 1 node"
mpiexec -f machinefile -n 4 ./examples/cpi

echo "\n**Running script for 2 nodes \n\n"
mpiexec -f machinefile -n 8 ./examples/cpi

echo "\n**Running script for 4 nodes \n\n"
mpiexec -f machinefile -n 16 ./examples/cpi

echo "\n**Running script for 8 node** \n\n"
mpiexec -f machinefile -n 32 ./examples/cpi