package ch.rihuber.noc.matching;

import ch.rihuber.noc.Node;

public class Vertex 
{
	
	private Node node;
	
	private int role;
	
	public static final int SRC_ROLE = 0;
	public static final int DEST_ROLE = 1;
	
	public Vertex(Node node, int role)
	{
		this.node = node;
		this.role = role;
	}

	public Node getNode() 
	{
		return node;
	}
	
	public String toString()
	{
		return Integer.toString(node.getId());
	}
	
	public boolean isSource()
	{
		return (role == SRC_ROLE);
	}
	

}
