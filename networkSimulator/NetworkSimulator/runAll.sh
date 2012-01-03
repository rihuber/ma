
# GRID TOPOLOGY

i=1
while [ $i -le 5 ]
do
  let nodes=$i*$i
  echo Calculating grid topology with $nodes nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t grid -s $nodes -f output/grid/$nodes
  let i=$i+1
done


# RING TOPOLOGY

i=1
while [ $i -le 25 ]
do
  echo Calculating ring topology with $i nodes
  java -classpath bin ch.rihuber.noc.NetworkSimulator -t ring -s $i -f output/ring/$i
  let i=$i+1
done

