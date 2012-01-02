package ch.rihuber.noc;
import java.util.HashMap;

import ch.rihuber.noc.router.Router;


public class Node 
{
	
	private int id;
	
	private HashMap<String, Link> outgoingLinks, incomingLinks;
	
	private Router router;
	
	public Node(int id, Router router) 
	{
		this.id = id;
		this.router = router;
		
		outgoingLinks = new HashMap<String, Link>();
		incomingLinks = new HashMap<String, Link>();
	}

	public int getId() 
	{
		return id;
	}

	public void addOutgoingLink(Link link, String name) 
	{
		outgoingLinks.put(name, link);
	}

	public void addIncomingLink(Link link, String name) 
	{
		incomingLinks.put(name, link);
	}

	public String toString()
	{
		String result = "Node " + id + "\n";
		return result;
	}

	public void forward(Node src, Node dest) 
	{
		if(dest == this)
			return;
		
		selectLink(dest).forward(src, dest);
	}
	
	private Link selectLink(Node dest)
	{
		String selectedLinkName = router.selectForwardingLink(this, dest);
		return outgoingLinks.get(selectedLinkName);
	}

}
