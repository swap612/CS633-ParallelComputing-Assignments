#! /bin/bash
#PBS -N job1
#PBS -q courses
#PBS -l nodes=16:ppn=1
#merge output and error into a single job_name.number_of_job_in_queue.
#PBS -j oe
#export fabric infiniband related variables
export I_MPI_FABRICS=shm:tmi
export I_MPI_DEVICE=rdma:OpenIB-cma
#change directory to where the job has been submitted from
cd $PBS_O_WORKDIR
#source paths
#source /opt/software/intel/initpaths intel64
#sort hostnames
sort $PBS_NODEFILE > hostfile
#run the job on required number of cores
# run with  N = {4, 16} and D = {1,4,16,64,256,512,1024}
for N in 4 16
do
    echo "Running for $N processes"
    rm -f plotdata_$N          # Remove the file if present
    
    for D in 1 4 16 64 256 512 1024
    do
        echo "Running for Data Size $D MB "
        # mpiexec -n $N ./src.x $D >> plotdata_$D                   #local testing
        # mpiexec -n $N -f machinefile ./src.x $D >> plotdata_$N      #cluster testing,
        # output is stored in plotdata file to be used to generate the plots
        mpirun -machinefile hostfile -np $N ./src.x $D >> plotdata_$N

    done
    
done
