package ch.rihuber.noc;

public class LinkLoad 
{
	
	private Node src, dest;
	
	public LinkLoad(Node src, Node dest) 
	{
		this.src = src;
		this.dest = dest;
	}
	
	public Node getSrc() 
	{
		return src;
	}

	public Node getDest() 
	{
		return dest;
	}
	
	public String toString()
	{
		String result = "( " + src.getId() + " -> " + dest.getId() + " )";
		return result;
	}
}