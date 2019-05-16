#! /bin/bash
#PBS -N job3.1
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

make
for nsc in 11 52 100
do
    echo "Running for nsc $nsc"
    #1st configuration 100 100 200 2 2 4
    N=16
    echo "Running for N = $N"
    #Removing the output files generated
    rm -f /scratch/$USER/*
    # Running 1st time with F so from next time the files will be available to read
    mpirun -machinefile hostfile -np $N ./s3d_io.x 100 100 200 2 2 4 1 F /scratch/$USER $nsc
    # Running 10 times
    for times in {1..10}
    do
      # running with T to calculate Read/Write Bandwidth
      mpirun -machinefile hostfile -np $N ./s3d_io.x 100 100 200 2 2 4 1 T /scratch/$USER $nsc > run.txt
      #Appending the output to main output.txt
      cat run.txt >> output.txt
      # doing text processing to retrieve the read/write bandwitdth
	    cat run.txt | grep 'read  bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >readfile
	    cat run.txt | grep 'write bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >writefile
      # Appending the nsc and number of processes to the bandwidth to generate the plottable input
      awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' readfile  >> read_plot
	    awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' writefile >> write_plot
    done

    #2nd configuration 100 200 200 2 4 4
    N=32
    echo "Running for N = $N"
    rm -f /scratch/$USER/*
    mpirun -machinefile hostfile -np $N ./s3d_io.x 100 200 200 2 4 4 1 F /scratch/$USER $nsc
    for times in {1..10}
    do
            mpirun -machinefile hostfile -np $N ./s3d_io.x 100 200 200 2 4 4 1 T /scratch/$USER $nsc > run.txt
            cat run.txt >> output.txt
            cat run.txt | grep 'read  bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >readfile
            cat run.txt | grep 'write bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >writefile
            awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' readfile  >> read_plot
            awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' writefile >> write_plot
    done

    #3rd configuration  200 200 200 4 4 4
    N=64
    echo "Running for N = $N"
    rm -f /scratch/$USER/*
    mpirun -machinefile hostfile -np $N ./s3d_io.x 200 200 200 4 4 4 1 F /scratch/$USER $nsc
    for times in {1..10}
    do
            mpirun -machinefile hostfile -np $N ./s3d_io.x 200 200 200 4 4 4 1 T /scratch/$USER $nsc > run.txt
            cat run.txt >> output.txt
            cat run.txt | grep 'read  bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >readfile
            cat run.txt | grep 'write bandwidth' | awk -F ":" '{print $2}' | sed 's/[MiB/s]/ /g; s/ //g' >writefile
            awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' readfile  >> read_plot
            awk -v nsc="$nsc" -v n="$N" '{print nsc"_"n ", "$0}' writefile >> write_plot
    done
done
