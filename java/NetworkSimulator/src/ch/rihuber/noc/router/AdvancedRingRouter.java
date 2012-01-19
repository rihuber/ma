package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.RingTopology;

public class AdvancedRingRouter extends Router 
{
	
	private int nodeCount;

	public AdvancedRingRouter(int nodeCount) 
	{
		this.nodeCount = nodeCount;
	}

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		int srcId = switchingNode.getId();
		int destId = dest.getId();
		if(destId > srcId)
			if(destId-srcId < nodeCount-destId+srcId)
				return RingTopology.FORWARD;
			else 
				return RingTopology.BACKWARD;
		else
			if(srcId-destId > nodeCount-srcId+destId)
				return RingTopology.FORWARD;
			else 
				return RingTopology.BACKWARD;
	}
	
	public String toString()
	{
		return "Advanced Ring Router";
	}

}
