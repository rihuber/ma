package ch.rihuber.noc;
import ch.rihuber.noc.load.Load;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;


public class NetworkSimulator 
{

	private final int GRID_SIZE = 4;
	
	private Link globalBottleneckLink;
	private float globalBottleneckLoadValue = 0;
	private Load globalBottleneckLoad;

	public static void main(String[] args) 
	{
		System.out.println("Starting simulation...");
		
		long startTime = System.currentTimeMillis();
		
		new NetworkSimulator().run();
		
		long endTime = System.currentTimeMillis();
		
		long timeUtilization = endTime - startTime;
		System.out.print("Simulation completed in " + timeUtilization + "ms");
	}

	private void run() 
	{
		Topology topology = new GridTopology(GRID_SIZE, Node.XY_ROUTING);
		
		Network network = new Network(topology);
		
		while(network.hasNextLoad())
		{
			network.nextLoad();
			Link localBottleneckLink = network.getBottleneckLink();
			if(globalBottleneckLoadValue < localBottleneckLink.getTotalLoad())
			{
				globalBottleneckLink = localBottleneckLink;
				globalBottleneckLoadValue = localBottleneckLink.getTotalLoad();
				globalBottleneckLoad = network.getCurrentLoad();
				
				System.out.println("With global load configuration:");
				System.out.println(network.getCurrentLoad());
				System.out.println("Found new bottleneck link:");
				System.out.println(globalBottleneckLink);
			}
		}
	}

//	private void printSummary() 
//	{
//		Link bottleNeckLink = linkList.getFirst();
//		for(Link currentLink : linkList)
//		{
//			if(currentLink.getTotalLoad() > bottleNeckLink.getTotalLoad())
//				bottleNeckLink = currentLink;
//		}
//		
//		System.out.println("Network bottleneck:");
//		System.out.println(bottleNeckLink);
//	}
//	
//	private Load createUpstreamLoad() 
//	{
//		LinkedList<Node> availableNodes = new LinkedList<Node>(nodeList);
//		Node hwInterfaceNode = availableNodes.pop();
//		Node swInterfaceNode = availableNodes.pop();
//		Load result = new Load(hwInterfaceNode, swInterfaceNode, availableNodes, 1);
//		
//		return result;
//	}
//
//	private void applyRouting() 
//	{
//		for(Node currentNode : nodeList)
//		{
//			currentNode.routeCurrentLoad();
//		}
//	}

}
