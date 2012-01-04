package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;
import ch.rihuber.noc.router.AdvancedRingRouter;
import ch.rihuber.noc.router.SimpleRingRouter;
import ch.rihuber.noc.router.Router;

public class RingTopology extends Topology 
{
	
	private int nodeCount;
	
	private Router router; 
	
	private static final String SIMPLE_RING_ROUTER = "simpleRingRouter";
	private static final String ADVANCED_RING_ROUTER = "advancedRingRouter";
	
	
	public final static String FORWARD = "Forward";
	public final static String BACKWARD = "Backward";

	
	public RingTopology(int nodeCount, String routerName)
	{
		this.nodeCount = nodeCount;
		
		if(routerName.equals(SIMPLE_RING_ROUTER))
			router = new SimpleRingRouter();
		else if(routerName.equals(ADVANCED_RING_ROUTER))
			router = new AdvancedRingRouter(nodeCount);
		
		init();
	}

	@Override
	protected LinkedList<Node> createNodeList() 
	{
		LinkedList<Node> result = new LinkedList<Node>();
		
		for(int i=0; i<nodeCount; i++)
			result.add(new Node(i, router));
		
		return result;
	}

	@Override
	protected LinkedList<Link> createLinkList() 
	{
		LinkedList<Link> result = new LinkedList<Link>();
		
		for(int i=0; i<nodeCount-1; i++)
		{
			result.add(createLink(i, RingTopology.FORWARD, i+1));
			result.add(createLink(i+1, RingTopology.BACKWARD, i));
		}
		result.add(createLink(nodeCount-1, RingTopology.FORWARD, 0));
		result.add(createLink(0, RingTopology.BACKWARD, nodeCount-1));
		
		return result;
	}

	public String toString()
	{
		String result = "R I N G  T O P O L O G Y\n" +
						"------------------------\n\n";
		
		result += "Ring size: " + nodeCount + "\n\n ";
		result += "Router: " + router.toString() + "\n\n";
		
		for(int i=0; i<nodeCount; i++)
		{
			result += "- " + i + " -";
		}
		result += "\n";
		result += "|";
		for(int i=0; i<nodeCount; i++)
		{
			int spaceCount;
			if(i<10)
				spaceCount = 5;
			else
				spaceCount = 6;
			for(int j=0; j<spaceCount; j++)
				result += " ";
		}
		result += "|\n ";
		for(int i=0; i<nodeCount; i++)
		{
			int spaceCount;
			if(i<10)
				spaceCount = 5;
			else
				spaceCount = 6;
			for(int j=0; j<spaceCount; j++)
				result += "-";
		}
		result += "\n\n\n";
		return result;
	}

}
