topDir=$PWD/output

# clear working directory
rm -r $topDir
mkdir $topDir

# prepare $topDir/summary file
printf "Nodes;" > $topDir/summary
i=1
while [ $i -le 25 ]
do
	printf "$i;" >> $topDir/summary
	let i=$i+1
done
printf "\n" >> $topDir/summary


# GRID TOPOLOGY
topology=grid
topologyDir=$topDir/$topology
mkdir $topologyDir

# XY Router
router=xyRouter
outputDir=$topologyDir/$router
mkdir $outputDir
printf "Grid;" >> $topDir/summary
i=1
gridSize=1
while [ $i -le 25 ]
do
  let currentGridSize=$gridSize*$gridSize
  if [ $i -eq $currentGridSize ]
  then
	  echo Calculating grid topology with $i nodes
	  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $i -f $outputDir/$i -r $router >> $topDir/summary
	  let gridSize=$gridSize+1
  fi
  printf ";" >> $topDir/summary
  let i=$i+1
done
printf "\n" >> $topDir/summary



# RING TOPOLOGY
topology=ring
topologyDir=$topDir/$topology
mkdir $topologyDir

printf "Ring Simple;" >> $topDir/summary
# Simple Ring Router
router=simpleRingRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 25 ]
do
  let nodes=$i
  echo Calculating $topology topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router >> $topDir/summary
  let i=$i+1
  printf ";" >> $topDir/summary
done
printf "\n" >> $topDir/summary

# Advanced Ring Router
printf "Ring Advanced;" >> $topDir/summary
router=advancedRingRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 25 ]
do
  let nodes=$i
  echo Calculating $topology topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router >> $topDir/summary
  let i=$i+1
  printf ";" >> $topDir/summary
done
printf "\n" >> $topDir/summary





# TORUS TOPOLOGY
topology=torus
topologyDir=$topDir/$topology
mkdir $topologyDir

# Simple Torus Router
printf "Torus Simple;" >> $topDir/summary
router=simpleTorusRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
gridSize=1
while [ $i -le 25 ]
do
  let currentGridSize=$gridSize*$gridSize
  if [ $i -eq $currentGridSize ]
  then
	  echo Calculating $topology topology with $i nodes
	  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $i -f $outputDir/$i -r $router >> $topDir/summary
	  let gridSize=$gridSize+1
  fi
  let i=$i+1
  printf ";" >> $topDir/summary
done
printf "\n" >> $topDir/summary



# SPIN TOPOLOGY
topology=spin
topologyDir=$topDir/$topology
mkdir $topologyDir

# Simple Torus Router
router=spinRouter
outputDir=$topologyDir/$router
mkdir $outputDir
fanout=2
while [ $fanout -le 13 ]
do
	i=1
	mkdir $outputDir/fanout$fanout 
	printf "Spin FO $fanout;" >> $topDir/summary
	while [ $i -le 25 ]
	do
	  nodes=$i
	  echo Calculating $topology topology with $nodes nodes and fanout $fanout
	  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/fanout$fanout/nodes$nodes -r $router --fanout $fanout >> $topDir/summary
	  let i=$i+1
	  printf ";" >> $topDir/summary
	done
	let fanout=$fanout+1
	printf "\n" >> $topDir/summary
done



