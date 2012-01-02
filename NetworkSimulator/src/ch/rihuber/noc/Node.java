package ch.rihuber.noc;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map.Entry;

import ch.rihuber.noc.router.Router;
import ch.rihuber.noc.router.XYRouter;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;


public class Node 
{
	
	public static final int XY_ROUTING = 0;
	
	private int id;
	
	private HashMap<String, Link> outgoingLinks, incomingLinks;
	
	private Hashtable<Node, Float> load;
	
	private Router router;
	
	public Node(int id, int routingPolicy, Topology topology) 
	{
		this.id = id;
		outgoingLinks = new HashMap<String, Link>();
		incomingLinks = new HashMap<String, Link>();
		
		load = new Hashtable<Node, Float>();
		
		if(routingPolicy == XY_ROUTING)
			router = new XYRouter((GridTopology) topology);
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

	public void addLoad(Node dest, float loadVolume) 
	{
		load.put(dest, loadVolume);
	}
	
	public String toString()
	{
		String result = "Node " + id + "\n";
		result += "Loads: ";
		Iterator<Entry<Node, Float>> iterator = load.entrySet().iterator();
		while(iterator.hasNext())
		{
			Entry<Node, Float> currentEntry = iterator.next();
			result += currentEntry.getKey().getId();
			result += " (";
			result += currentEntry.getValue();
			result += ") ";
			if(iterator.hasNext())
				result += ", ";
		}
		return result;
	}

	public void routeCurrentLoad() 
	{
		for(Entry<Node, Float> currentEntry : load.entrySet())
		{
			Node dest = currentEntry.getKey();
			selectLink(dest).forward(this, currentEntry.getKey(), currentEntry.getValue());
		}
	}

	public void forward(Node src, Node dest, Float loadValue) 
	{
		if(dest == this)
			return;
		
		selectLink(dest).forward(src, dest, loadValue);
	}
	
	private Link selectLink(Node dest)
	{
		String selectedLinkName = router.selectForwardingLink(this, dest);
		return outgoingLinks.get(selectedLinkName);
	}

	public void clearLoad() 
	{
		load.clear();
	}

}
