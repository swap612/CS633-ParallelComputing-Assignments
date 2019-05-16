#script to generate machinefile

#remove prev available machine file
rm -f machinefile

count=0
i=1
while [ $i -le 30 ]; do
  host=172.27.19.$i
  ping -c 1 $host > /dev/null
  if [ $? -eq 0 ]; then
    echo $host:4 >> machinefile
    count=`expr $count + 1`
  fi

  if [ $count -eq 8 ]; then
    break
  fi
  i=`expr $i + 1`
done
