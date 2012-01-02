package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;

public abstract class Topology 
{
	
	// All routers in the network
	private LinkedList<Node> nodeList;
	
	// All active links between nodes
	private LinkedList<Link> linkList;
	
	protected void init()
	{
		nodeList = createNodeList();
		linkList = createLinkList();
	}
	
	protected abstract LinkedList<Node> createNodeList();
	
	protected abstract LinkedList<Link> createLinkList();
	
	protected Link createLink(int startId, String startNodeLinkName, int endId, String endNodeLinkName) 
	{
		Node startNode = null, endNode = null;
		for(Node currentNode : nodeList)
		{
			if(currentNode.getId() == startId)
				startNode = currentNode;
			if(currentNode.getId() == endId)
				endNode = currentNode;
			if(startNode != null && endNode != null)
				break;
		}
		Link result = new Link(startNode, startNodeLinkName, endNode, endNodeLinkName);
		return result;
	}

	public Link getBottleneckLink() 
	{
		Link bottleNeckLink = linkList.getFirst();
		for(Link currentLink : linkList)
		{
			if(currentLink.getTotalLoad() > bottleNeckLink.getTotalLoad())
				bottleNeckLink = currentLink;
		}
		return bottleNeckLink;
	}

	public LinkedList<Node> getNodes() 
	{
		return nodeList;
	}

	public void clearLoads() 
	{
		for(Node currentNode : nodeList)
		{
			currentNode.clearLoad();
		}
		
		for(Link currentLink : linkList)
		{
			currentLink.clearLoads();
		}
	
			
		
				
	}

	public void routeCurrentLoads() 
	{
		for(Node currentNode : nodeList)
		{
			currentNode.routeCurrentLoad();
		}
	}

}
