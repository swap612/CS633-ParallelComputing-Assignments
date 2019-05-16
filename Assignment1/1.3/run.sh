# Steps for running 1.3
echo "*****Running Assignment 1.3****"
# 1. make executables 
echo "Making executables"
make -s           

#Generate the machinefile
echo "Generating Machinefile"
./gen_machinefile.sh

# run with  D=1, 512, 1024 & N= {8, 16, 32, 64, 128}
for D in 1 512 1024
do
    echo "Running for Data Size $D KB"
    rm -f plotdata_$D          # Remove the file if present
    for N in 8 16 32 64 128
    do 
        echo "Running for $N processes"       
        # mpiexec -n $N ./src.x $D >> plotdata_$D                   #local testing  
        # output is stored in plotdata file to be used to generate the plots 
        mpiexec -n $N -f machinefile ./src.x $D >> plotdata_$D      #cluster testing, 
    done

done

# Plot graph
echo "Ploting Graphs"
for D in 1 512 1024                     # for D={, 512, 1024}
do
    python3 plot.py plotdata_$D         # plot.py takes the plotdata file as arg
done 
