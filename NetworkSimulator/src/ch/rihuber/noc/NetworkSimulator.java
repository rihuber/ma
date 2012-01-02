package ch.rihuber.noc;
import java.util.LinkedList;

import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;


public class NetworkSimulator 
{

	private final int GRID_SIZE = 4;

	public static void main(String[] args) 
	{
		System.out.println("Starting calculation");
		
		long startTime = System.currentTimeMillis();
		
		new NetworkSimulator().run();
		
		long endTime = System.currentTimeMillis();
		
		long timeUtilization = endTime - startTime;
		System.out.print("Calculation completed in " + timeUtilization + "ms");
	}

	private void run() 
	{
		
		Topology topology = new GridTopology(GRID_SIZE, GridTopology.XY_ROUTING);
		
		sendExplorerPackages(topology);
	}

	private void sendExplorerPackages(Topology topology) 
	{
		LinkedList<Node> nodes = topology.getNodes();
		for(Node sourceNode : nodes)
		{
			for(Node destNode : nodes)
			{
				sourceNode.forward(sourceNode, destNode);
			}
		}
	}
}