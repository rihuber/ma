package ch.rihuber.noc.matching;

public class Edge 
{
	
	private Vertex vertex1, vertex2;
	
	public Edge(Vertex vertex1, Vertex vertex2)
	{
		this.vertex1 = vertex1;
		this.vertex2 = vertex2;
	}

	public boolean isIncident(Vertex v) 
	{
		return (v == vertex1 || v == vertex2);
	}

	public Vertex getAdj(Vertex v) 
	{
		if(v == vertex1)
			return vertex2;
		return vertex1;
	}
	
	public String toString()
	{
		return vertex1 + "->" + vertex2;
	}

}
