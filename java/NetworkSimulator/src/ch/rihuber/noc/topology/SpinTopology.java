package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;
import ch.rihuber.noc.router.Router;
import ch.rihuber.noc.router.SpinRouter;

public class SpinTopology extends Topology 
{
	
	private int fanout;
	private int nodeCount;
	private int parentNodeCount;
	
	private Router router; 
	
	public final static String SPIN_ROUTER = "spinRouter";
	public final static String UPSTREAM = 	"upstream";
	public final static String FORWARD = 	"forward";
	
	public SpinTopology(int nodeCount, int fanout, String routerName)
	{
		this.nodeCount = nodeCount;
		this.fanout = fanout;
		parentNodeCount = (int)Math.ceil((double)nodeCount/(double)fanout);
		
		if(routerName.equals(SPIN_ROUTER))
			router = new SpinRouter(nodeCount, fanout);
		
		
		init();
	}

	@Override
	protected LinkedList<Node> createNodeList() 
	{
		LinkedList<Node> result = new LinkedList<Node>();
		
		// Leaf nodes
		for(int i=0; i<nodeCount; i++)
		{
			result.add(new Node(i, router));
		}
		
		// Parent nodes
		for(int i=0; i<parentNodeCount; i++)
		{
			result.add(new Node(i+nodeCount, router));
		}
		
		return result;
	}

	@Override
	protected LinkedList<Link> createLinkList() 
	{
		LinkedList<Link> result = new LinkedList<Link>();
		
		// Links to and from leafs
		for(int i=0; i<nodeCount; i++)
		{
			int parentNodeId = i/fanout + nodeCount;
			int downstreamLink = i % fanout;
			result.add(createLink(i, SpinTopology.UPSTREAM, parentNodeId));
			result.add(createLink(parentNodeId, Integer.toString(downstreamLink), i));
		}
		
		// Links interconnecting parents
		for(int i=0; i<parentNodeCount-1; i++)
		{
			result.add(createLink(i+nodeCount, SpinTopology.FORWARD, i+nodeCount+1));
		}
		result.add(createLink(nodeCount+parentNodeCount-1, SpinTopology.FORWARD, nodeCount));

		return result;
	}
	
	public String toString()
	{
		String result = "S P I N   T O P O L O G Y\n" +
						"-------------------------\n\n";
		
		result += "IP blocks: " + nodeCount + "\n";
		result += "Fanout: " + fanout + "\n\n\n\n";

		result += "\n    _";
		for(int i=0; i<parentNodeCount; i++)
		{
			result += "_____";
			for(int a=0; a<fanout-1; a++)
			{
				result += "____";
			}
			result += "_____";
		}
		result += "__";
		
		result += "\n   | ";
		for(int i=0; i<parentNodeCount; i++)
		{
			result += "    _";
			for(int a=0; a<fanout-1; a++)
			{
				result += "____";
			}
			result += "__   ";
		}
		result += "  |";
		
		result += "\n   |_";
		for(int i=0; i<parentNodeCount; i++)
		{
			result += "___|   ";
			if(i+nodeCount<10)
				result += " ";
			result += (i+nodeCount);
			for(int a=0; a<fanout-2; a++)
			{
				result += "    ";
			}
			result += "  |__";
		}
		result += "__|";
		
		result += "\n     ";
		for(int i=0; i<parentNodeCount; i++)
		{
			result += "   |_";
			for(int a=0; a<fanout-1; a++)
			{
				result += "____";
			}
			result += "__|  ";
		}
		result += "  ";
		
		
		
//		result += "\n\n";
//		result += "\n    =";
//		for(int i=0; i<parentNodeCount; i++)
//		{
//			result += "---__";
//			for(int a=0; a<fanout-1; a++)
//			{
//				result += "|___";
//			}
//			result += "|__--";
//		}
//		result += "==";

		
		for(int j=0; j<2; j++)
		{
			result += "\n    ";
			for(int i=0; i<parentNodeCount; i++)
			{
				result += "   ";
				for(int a=0; a<fanout; a++)
				{
					result += "   |";
				}
				result += "   ";
			}
		}
		result += "\n    ";
		for(int i=0; i<parentNodeCount; i++)
		{
			result += "   ";
			for(int a=0; a<fanout; a++)
			{
				result += "  ";
				int nodeId = a + i*fanout;
				if(nodeId<10)
					result += " ";
				if(nodeId<nodeCount)
					result += nodeId;
			}
			result += "   ";
		}
		
		
		
		result += "\n\n\n\n\n";
		
		return result;
	}
	
	@Override
	public void sendExplorerPackages() 
	{
		for(Node srcNode : getNodes())
		{
			for(Node destNode : getNodes())
			{
				if(srcNode.getId() < nodeCount && destNode.getId() < nodeCount)
					srcNode.forward(srcNode, destNode);
			}
		}
	}

}
