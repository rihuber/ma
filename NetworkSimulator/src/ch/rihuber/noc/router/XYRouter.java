package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.GridTopology;

public class XYRouter extends Router 
{

	private int id;
	
	private GridTopology gridTopology;
	
	public XYRouter(int id, GridTopology gridTopology) 
	{
		this.id = id;
		this.gridTopology = gridTopology;
	}

	@Override
	public String selectForwardingLink(Node dest) 
	{
		int destId = dest.getId();
		int gridSize = gridTopology.getGridSize();

		if (destId % gridSize > id % gridSize)
			return GridTopology.EAST;
		if (destId % gridSize < id % gridSize)
			return GridTopology.WEST;
		if (destId > id)
			return GridTopology.SOUTH;
		// if(destId < id)
			return GridTopology.NORTH;
	}

}
