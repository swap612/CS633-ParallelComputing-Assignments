#! /bin/bash
#PBS -N job3.3
#PBS -q courses
#PBS -l nodes=8:ppn=8
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

for nsc in 11 52 100
do
    echo "Running for nsc $nsc"
    #1st configuration
    N=64
    echo "Running for N = $N"
    rm -f /scratch/$USER/*
    mpirun -machinefile hostfile -np $N ./s3d_io.x 400 400 400 4 4 4 1 F /scratch/$USER $nsc
    for times in {1..10}
    do
            mpirun -machinefile hostfile -np $N ./s3d_io.x 400 400 400 4 4 4 1 T /scratch/$USER $nsc > run.txt
	    cat run.txt >> output.txt
	    cat run.txt | grep 'read  bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >readfile
	    cat run.txt | grep 'write bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >writefile
	    awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' readfile  >> read_plot
	    awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' writefile >> write_plot
    done
done
