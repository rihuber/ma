package ch.rihuber.noc;
import java.util.LinkedList;

import ch.rihuber.noc.matching.Edge;
import ch.rihuber.noc.matching.HopcroftKarp;
import ch.rihuber.noc.matching.MatchingResult;
import ch.rihuber.noc.matching.Vertex;


public class Link 
{
	
	private Node startNode, endNode;
	
	private LinkedList<LinkLoad> linkLoads;
	
	public Link(Node startNode, Node endNode)
	{
		this.startNode = startNode;
		this.endNode = endNode;
		
		linkLoads = new LinkedList<LinkLoad>();
	}
	
	public String toString()
	{
		String result = "Link " + startNode.getId() + " -> " + endNode.getId();
		
		// DEBUG OUTPUT
//		result += "Packets routed over this link:\t";
//		for(LinkLoad currentLoad : linkLoads)
//		{
//			result += currentLoad + "\t";
//		}
//		
//		result += "\nSet of senders:\t";
//		for(Node currentNode : getSenders())
//		{
//			result += currentNode.getId() + "  ";
//		}
//		
//		result += "\nSet of receivers:\t";
//		for(Node currentNode : getReceiver())
//		{
//			result += currentNode.getId() + "  ";
//		}
		
		return result;
	}

	private LinkedList<Edge> createEdges(LinkedList<Vertex> senderVertices, LinkedList<Vertex> receiverVertices, LinkedList<LinkLoad> linkLoads) 
	{
		LinkedList<Edge> result = new LinkedList<Edge>();
		
		for(LinkLoad currentLinkLoad : linkLoads)
		{
			Node srcNode = currentLinkLoad.getSrc();
			Vertex srcVertex = null;
			for(Vertex currentVertex : senderVertices)
			{
				if(srcNode == currentVertex.getNode())
				{
					srcVertex = currentVertex;
					break;
				}
			}
			
			Node destNode = currentLinkLoad.getDest();
			Vertex destVertex = null;
			for(Vertex currentVertex : receiverVertices)
			{
				if(destNode == currentVertex.getNode())
				{
					destVertex = currentVertex;
					break;
				}
			}
			
			result.add(new Edge(srcVertex, destVertex));
		}
		
		return result;
	}

	private LinkedList<Vertex> createVertices(LinkedList<Node> nodes, int role) 
	{
		LinkedList<Vertex> result = new LinkedList<Vertex>();
		for(Node currentNode : nodes)
		{
			result.add(new Vertex(currentNode, role));
		}
		return result;
	}

	private LinkedList<Node> getSenders() 
	{
		LinkedList<Node> result = new LinkedList<Node>();
		for(LinkLoad load : linkLoads)
		{
			if(!result.contains(load.getSrc()))
				result.add(load.getSrc());
		}
		return result;
	}
	
	private LinkedList<Node> getReceiver() 
	{
		LinkedList<Node> result = new LinkedList<Node>();
		for(LinkLoad load : linkLoads)
		{
			if(!result.contains(load.getDest()))
				result.add(load.getDest());
		}
		return result;
	}

	public void forward(Node src, Node dest) 
	{
		linkLoads.add(new LinkLoad(src, dest));	
		endNode.forward(src, dest);
	}

	public MatchingResult getMatchingResult() 
	{
		LinkedList<Vertex> senderVertices = createVertices(getSenders(), Vertex.SRC_ROLE);
		LinkedList<Vertex> receiverVertices = createVertices(getReceiver(), Vertex.DEST_ROLE);
		LinkedList<Edge> edges = createEdges(senderVertices, receiverVertices, linkLoads);
		
		HopcroftKarp matchingCalculator = new HopcroftKarp(senderVertices, receiverVertices, edges);
		return new MatchingResult(matchingCalculator);		
	}

}
