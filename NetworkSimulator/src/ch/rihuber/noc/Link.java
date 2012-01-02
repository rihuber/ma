package ch.rihuber.noc;
import java.util.LinkedList;


public class Link 
{
	
	private Node startNode, endNode;
	
	private LinkedList<LinkLoad> linkLoads;
	
	private class LinkLoad
	{
		public Node src;
		public Node dest;

		public LinkLoad(Node src, Node dest) 
		{
			this.src = src;
			this.dest = dest;
		}
	}
	
	public Link(Node startNode, Node endNode)
	{
		this.startNode = startNode;
		this.endNode = endNode;
	}
	
	public String toString()
	{
		String result = "Link " + startNode.getId() + " -> " + endNode.getId() + "\n";
		return result;
	}

	public void forward(Node src, Node dest) 
	{
		linkLoads.add(new LinkLoad(src, dest));	
		endNode.forward(src, dest);
	}

}
