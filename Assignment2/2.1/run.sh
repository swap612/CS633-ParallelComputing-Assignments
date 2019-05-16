#! /bin/bash
#PBS -N job1
#PBS -q courses
#PBS -l nodes=16:ppn=8
#merge output and error into a single job_name.number_of_job_in_queue.
#PBS -j oe
#export fabric infiniband related variables
export I_MPI_FABRICS=shm:tmi
export I_MPI_DEVICE=rdma:OpenIB-cma
#change directory to where the job has been submitted from
cd $PBS_O_WORKDIR
#source paths
source /opt/software/intel/initpaths intel64
#sort hostnames
sort $PBS_NODEFILE > hostfile
# Running the tasks
for D in 1 512 1024
do
    echo "Running for Data Size $D KB"
    rm -f plotdata_$D          # Remove the file if present
    for imp in 0 1
    do 
        for N in 8 16 32 64 128
        do  
            echo "Running for $N processes"       
            # mpiexec -n $N ./src.x $D >> plotdata_$D                           #local testing  
            # mpiexec -n $N -f machinefile ./src.x $D $imp >> plotdata_$D      #cluster testing 
            # output is stored in plotdata file to be used to generate the plots 
            mpirun -machinefile hostfile -np $N ./src.x $D $imp >> plotdata_$D
	        
        done
    done
done

 
