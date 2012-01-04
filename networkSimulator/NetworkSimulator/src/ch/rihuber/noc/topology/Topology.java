package ch.rihuber.noc.topology;

import java.util.LinkedList;

import ch.rihuber.noc.Link;
import ch.rihuber.noc.Node;

public abstract class Topology 
{
	
	// All switches in the network
	private LinkedList<Node> nodeList;
	
	// All links between nodes
	private LinkedList<Link> linkList;
	
	protected void init()
	{
		nodeList = createNodeList();
		linkList = createLinkList();
	}
	
	protected abstract LinkedList<Node> createNodeList();
	
	protected abstract LinkedList<Link> createLinkList();
	
	protected Link createLink(int startId, String startNodeLinkName, int endId) 
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
		Link result = new Link(startNode, endNode);
		startNode.addOutgoingLink(result, startNodeLinkName);
		return result;
	}

	public LinkedList<Node> getNodes() 
	{
		return nodeList;
	}

	public LinkedList<Link> getLinks() 
	{
		return linkList;
	}
	
	public void sendExplorerPackages() 
	{
		for(Node sourceNode : getNodes())
		{
			for(Node destNode : getNodes())
			{
				sourceNode.forward(sourceNode, destNode);
			}
		}
	}

}
