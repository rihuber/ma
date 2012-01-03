package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;
import ch.rihuber.noc.router.RingRouter;
import ch.rihuber.noc.router.Router;

public class RingTopology extends Topology 
{
	
	private int nodeCount;
	
	private Router router; 
	
	public final static String FORWARD = "Forward";
	
	public RingTopology(int nodeCount)
	{
		this.nodeCount = nodeCount;
		
		router = new RingRouter();
		
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
		}
		result.add(createLink(nodeCount-1, RingTopology.FORWARD, 0));
		
		return result;
	}

	public String toString()
	{
		String result = "R I N G  T O P O L O G Y\n" +
						"------------------------\n\n";
		
		result += "Ring size: " + nodeCount + "\n\n ";
		
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
