package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.TorusTopology;

public class SimpleTorusRouter extends Router 
{

	private int xSize;
	
	public SimpleTorusRouter(int xSize) 
	{
		this.xSize = xSize;
	}

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		int destId = dest.getId();
		int switchingNodeId = switchingNode.getId();

		if (destId % xSize != switchingNodeId % xSize)
			return TorusTopology.VERTICAL_FORWARD;
		if (destId != switchingNodeId)
			return TorusTopology.HORIZONTAL_FORWARD;
		
		return null;
	}
	
	public String toString()
	{
		return "Simple Torus Router";
	}


}
