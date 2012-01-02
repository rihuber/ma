package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;
import ch.rihuber.noc.router.Router;
import ch.rihuber.noc.router.XYRouter;

public class GridTopology extends Topology 
{
	
	private int gridSize;
	private int nodeCount;
	
	private Router router; 
	
	public final static String NORTH = "North";
	public final static String SOUTH = "South";
	public final static String WEST = "West";
	public final static String EAST = "East";
	
	public static final int XY_ROUTING = 0;
	
	public GridTopology(int gridSize, int routingPolicy)
	{
		this.gridSize = gridSize;
		this.nodeCount = gridSize * gridSize;
		
		if(routingPolicy == XY_ROUTING)
			router = new XYRouter(gridSize);
		
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
		
		for(int i=0; i<nodeCount; i++)
		{
			// north
			if(i >= gridSize)
				result.add(createLink(i, GridTopology.NORTH, i-gridSize));
			
			// south
			if(i+gridSize < (nodeCount))
				result.add(createLink(i, GridTopology.SOUTH, i+gridSize));
			
			// west
			if(i%gridSize != 0)
				result.add(createLink(i, GridTopology.WEST, i-1));
			
			// east
			if(i%gridSize != gridSize-1)
				result.add(createLink(i, GridTopology.EAST, i+1));
		}
		
		return result;
	}

	public int getGridSize() 
	{
		return gridSize;
	}

}
