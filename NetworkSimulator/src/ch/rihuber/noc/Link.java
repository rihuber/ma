package ch.rihuber.noc;
import java.util.LinkedList;


public class Link 
{
	
	private Node startNode, endNode;
	
	private class LinkLoad
	{
		public Node src;
		public Node dest;
		public float loadValue;

		public LinkLoad(Node src, Node dest, Float loadValue) 
		{
			this.src = src;
			this.dest = dest;
			this.loadValue = loadValue;
		}
		
		public String toString()
		{
			return loadValue + " (" + src.getId() + "->" + dest.getId() + ")";
		}
	}
	
	private LinkedList<LinkLoad> linkLoads;
	
	public Link(Node startNode, String startNodeLinkName, Node endNode, String endNodeLinkName)
	{
		this.startNode = startNode;
		this.endNode = endNode;
		
		startNode.addOutgoingLink(this, startNodeLinkName);
		endNode.addIncomingLink(this, endNodeLinkName);
		
		linkLoads = new LinkedList<LinkLoad>();
	}
	
	public String toString()
	{
		String result = "Link " + startNode.getId() + " -> " + endNode.getId() + "\n";
		
		result += "Load: ";
		for(LinkLoad currentLinkLoad : linkLoads)
		{
			result += currentLinkLoad;
		}
		
		result += "\nTotal: " + getTotalLoad() + "\n\n";
		
		return result;
	}

	public void forward(Node src, Node dest, Float loadValue) 
	{
		linkLoads.add(new LinkLoad(src, dest, loadValue));	
		endNode.forward(src, dest, loadValue);
	}

	public float getTotalLoad() 
	{
		float total = 0;
		for(LinkLoad currentLinkLoad : linkLoads)
		{
			total += currentLinkLoad.loadValue;
		}
		return total;
	}

	public void clearLoads() 
	{
		linkLoads.clear();
	}

}
