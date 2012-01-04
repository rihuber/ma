package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.SpinTopology;

public class SpinRouter extends Router 
{
	private int fanout;
	private int nodeCount;

	public SpinRouter(int nodeCount, int fanout) 
	{
		this.nodeCount = nodeCount;
		this.fanout = fanout;
	}

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		int srcId = switchingNode.getId();
		int destId = dest.getId();
		
		// Source is leaf node
		if(srcId < nodeCount)
			return SpinTopology.UPSTREAM;
		
		// Parent node of dest
		int parentNodeIdOfDest = destId/fanout + nodeCount;
		if(parentNodeIdOfDest == srcId)
		{
			int downstreamLinkNumber = destId % fanout;
			return Integer.toString(downstreamLinkNumber);
		}
		
		return SpinTopology.FORWARD;
	}

}
