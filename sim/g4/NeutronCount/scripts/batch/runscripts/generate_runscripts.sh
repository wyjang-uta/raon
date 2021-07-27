#!/bin/bash

MACROFILE="POT1000.mac"
nruns=100
proccnt=0
for (( i=1; i<=nruns; i++))
do
  outfile="run${i}.sh"
  echo -e "#!/bin/bash\n" > $outfile
  echo -e "\n" >> $outfile
  echo -e "WORKDIR=/cluster/home/wyjang/raon/sim/g4/NeutronCount/build\n" >> $outfile
  # Here the variable i in the following sentence is the RANDOM SEED in the simulation!
  echo -e '$WORKDIR'"/exampleB1 "'$WORKDIR'/"POT1e6.mac $i" >> $outfile
  proccnt=$((proccnt+1))
done

echo "$proccnt macro files are generated."
