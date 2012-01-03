package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.GridTopology;

public class XYRouter extends Router 
{

	private int xSize;
	
	public XYRouter(int xSize) 
	{
		this.xSize = xSize;
	}

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		int destId = dest.getId();
		int switchingNodeId = switchingNode.getId();

		if (destId % xSize > switchingNodeId % xSize)
			return GridTopology.EAST;
		if (destId % xSize < switchingNodeId % xSize)
			return GridTopology.WEST;
		if (destId > switchingNodeId)
			return GridTopology.SOUTH;
		if(destId < switchingNodeId)
			return GridTopology.NORTH;
		
		return null;
	}

}
