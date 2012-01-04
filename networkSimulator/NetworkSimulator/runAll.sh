topDir=$PWD/output

# clear working directory
rm -r $topDir
mkdir $topDir


# GRID TOPOLOGY
topology=grid
topologyDir=$topDir/$topology
mkdir $topologyDir

# XY Router
router=xyRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 5 ]
do
  let nodes=$i*$i
  echo Calculating grid topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router
  let i=$i+1
done




# RING TOPOLOGY
topology=ring
topologyDir=$topDir/$topology
mkdir $topologyDir

# Simple Ring Router
router=simpleRingRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 25 ]
do
  let nodes=$i
  echo Calculating $topology topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router
  let i=$i+1
done

# Advanced Ring Router
router=advancedRingRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 25 ]
do
  let nodes=$i
  echo Calculating $topology topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router
  let i=$i+1
done






# TORUS TOPOLOGY
topology=torus
topologyDir=$topDir/$topology
mkdir $topologyDir

# Simple Torus Router
router=simpleTorusRouter
outputDir=$topologyDir/$router
mkdir $outputDir
i=1
while [ $i -le 5 ]
do
  let nodes=$i*$i
  echo Calculating $topology topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/$nodes -r $router
  let i=$i+1
done




# TORUS TOPOLOGY
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
	i=$fanout
	mkdir $outputDir/fanout$fanout 
	while [ $i -le 25 ]
	do
	  nodes=$i
	  echo Calculating $topology topology with $nodes nodes and fanout $fanout
	  java -classpath bin ch.rihuber.noc.NetworkSimulator -t $topology -s $nodes -f $outputDir/fanout$fanout/nodes$nodes -r $router --fanout $fanout
	  let i=$i+1
	done
	let fanout=$fanout+1
done

