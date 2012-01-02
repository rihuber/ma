package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;

public class XYRouter extends Router 
{

	private GridTopology gridTopology;
	
	public XYRouter(GridTopology gridTopology) 
	{
		this.gridTopology = gridTopology;
	}

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		int destId = dest.getId();
		int switchingNodeId = switchingNode.getId();
		int gridSize = gridTopology.getGridSize();

		if (destId % gridSize > switchingNodeId % gridSize)
			return GridTopology.EAST;
		if (destId % gridSize < switchingNodeId % gridSize)
			return GridTopology.WEST;
		if (destId > switchingNodeId)
			return GridTopology.SOUTH;
		if(destId < switchingNodeId)
			return GridTopology.NORTH;
		
		return Topology.SINK;
	}

}
