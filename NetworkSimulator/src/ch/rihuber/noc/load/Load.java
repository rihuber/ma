package ch.rihuber.noc.load;
import java.util.LinkedList;

import ch.rihuber.noc.Node;


public class Load 
{

	private LinkedList<Node> nodes;
	private float loadVolume;
	
	public Load(LinkedList<Node> nodes, float loatVolume) 
	{
		this.nodes = nodes;
		this.loadVolume = loatVolume;
	}

	public void apply() 
	{
		Node src = null, dest = null;
		for(Node currentNode : nodes)
		{
			src = dest;
			dest = currentNode;
			if(src != null && dest != null)
			{
				src.addLoad(dest, loadVolume);
			}
		}
	}
	
	public String toString()
	{
		String result = "Load (" + loadVolume + "): ";
		for(Node currentNode : nodes)
		{
			result += " -> " + currentNode.getId();
		}
		return result;
	}

}
