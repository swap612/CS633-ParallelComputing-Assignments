# Steps for running 2.1
# Plot graph
echo "Ploting Graphs"
for D in 1 512 1024                     # for D={1, 512, 1024}
do
    python3 plot.py plotdata_$D         # plot.py takes the plotdata file as arg
done
