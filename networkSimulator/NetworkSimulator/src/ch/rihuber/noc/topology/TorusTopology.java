package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;
import ch.rihuber.noc.router.Router;
import ch.rihuber.noc.router.SimpleTorusRouter;

public class TorusTopology extends Topology 
{

	private int gridSize;
	private int nodeCount;
	
	private Router router; 
	
	public final static String HORIZONTAL_FORWARD = "Horizontal forward";
	public final static String HORIZONTAL_BACKWARD = "Horizontal backward";
	public final static String VERTICAL_FORWARD = "Vertical forward";
	public final static String VERTICAL_BACKWARD = "Vertical backward";
	
	private final static String SIMPLE_TORUS_ROUTER = "simpleTorusRouter";
	
	public TorusTopology(int nodeCount, String routerName)
	{
		this.gridSize = (int) Math.sqrt(nodeCount);
		this.nodeCount = gridSize * gridSize;
		
		if(routerName.equals(SIMPLE_TORUS_ROUTER))
			router = new SimpleTorusRouter(gridSize);
		
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
			int destNode;
			
			// vertical backward
			if(i >= gridSize)
				destNode = i-gridSize;
			else
				destNode = i-gridSize+nodeCount;
			result.add(createLink(i, TorusTopology.VERTICAL_BACKWARD, destNode));
			
			// vertical forward
			if(i+gridSize < (nodeCount))
				destNode = i+gridSize;
			else
				destNode = i+gridSize-nodeCount;
			result.add(createLink(i, TorusTopology.HORIZONTAL_FORWARD, destNode));
			
			// horizontal backward
			if(i%gridSize != 0)
				destNode = i-1;
			else
				destNode = i + gridSize - 1;
			result.add(createLink(i, TorusTopology.VERTICAL_BACKWARD, destNode));
			
			// horizontal forward
			if(i%gridSize != gridSize-1)
				destNode = i + 1;
			else
				destNode = i - gridSize + 1;
			result.add(createLink(i, TorusTopology.VERTICAL_FORWARD, destNode));
		}
		
		return result;
	}

	public int getGridSize() 
	{
		return gridSize;
	}
	
	public String toString()
	{
		String result = "T O R U S   T O P O L O G Y\n" +
						"---------------------------\n\n";
		
		result += "Grid size: " + gridSize + "\n\n";
		result += "Router: " + router.toString() + "\n\n";
		
		for(int i=0; i<gridSize; i++)
			result += "    __";
		for(int j=0; j<gridSize; j++)
		{
			result += "\n __";
			for(int i=0; i<gridSize; i++)
				result += "|__:__";
			result += "\n|  |";
			for(int i=0; i<gridSize; i++)
				result += "  :  |";
			result += "\n -";
			for(int i=0; i<gridSize; i++)
			{
				int nodeId = (j*gridSize+i); 
				if(nodeId<10)
					result += "-";
				result += nodeId+"--:-";
			}
			result += "-\n ";
			for(int i=0; i<gridSize; i++)
				result += "  |  :";
		}
		result += "\n";
		for(int i=0; i<gridSize; i++)
			result += "    --";
		
		result += "\n\n";
		return result;
	}

}
