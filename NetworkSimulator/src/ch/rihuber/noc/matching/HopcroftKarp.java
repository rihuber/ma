package ch.rihuber.noc.matching;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map.Entry;

public class HopcroftKarp 
{
	
	private LinkedList<Vertex> srcNodes, nodeUnion;
	private LinkedList<Edge> edges;
	private Vertex nullVertex;
	
	private HashMap<Vertex, Vertex> pairs;
	private HashMap<Vertex, Integer> distances;
	private int matching;
	private LinkedList<Vertex> queue;
	
	public HopcroftKarp(LinkedList<Vertex> srcNodes, LinkedList<Vertex> destNodes, LinkedList<Edge> edges)
	{
		this.srcNodes = srcNodes;
		this.edges = edges;
		nullVertex = new NullVertex();

		nodeUnion = new LinkedList<Vertex>();
		nodeUnion.addAll(srcNodes);
		nodeUnion.addAll(destNodes);
		nodeUnion.add(nullVertex);
		
		pairs = new HashMap<Vertex, Vertex>();
		distances = new HashMap<Vertex, Integer>();
		
		queue = new LinkedList<Vertex>();
		
		runCalculation();
	}
	
	public int getMatchingEdgesCount()
	{
		return matching;
	}
	
	public HashMap<Vertex, Vertex> getPairs()
	{
		return pairs;
	}
	
	private void runCalculation() 
	{
		for(Vertex v : nodeUnion)
		{
			pairs.put(v, nullVertex);
		}
		matching = 0;
		
		while(breathFirstSearch() == true)
		{
			for(Vertex v : srcNodes)
			{
				if(pairs.get(v) == nullVertex)
				{
					if(depthFirstSearch(v) == true)
						matching++;
				}
			}
		}
		
		// cleanup
		// Remove entries with key nullVertex
		pairs.remove(nullVertex);
		// Remove entries with value nullVertex and remove entries from dest to src		
		Iterator<Entry<Vertex, Vertex>> iterator = pairs.entrySet().iterator();
		while(iterator.hasNext())
		{
			Entry<Vertex, Vertex> currentEntry = iterator.next();
			if(currentEntry.getValue() == nullVertex || currentEntry.getValue().isSource())
			{
				iterator.remove();
				continue;
			}
		}
	}

	private boolean depthFirstSearch(Vertex v) 
	{
		if(v != nullVertex)
		{
			for(Vertex u : getAdj(v))
			{
				if(distances.get(pairs.get(u)) == distances.get(v)+1)
				{
					if(depthFirstSearch(pairs.get(u)) == true)
					{
						pairs.put(u, v);
						pairs.put(v, u);
						return true;
					}
				}
			}
			distances.put(v, -1);
			return false;
		}
		return true;
	}

	private boolean breathFirstSearch() 
	{
		for(Vertex v : srcNodes)
		{
			if(pairs.get(v) == nullVertex)
			{
				distances.put(v, 0);
				queue.addLast(v);
			}
			else
				distances.put(v, -1);
		}
		
		distances.put(nullVertex, -1);
		
		while(!queue.isEmpty())
		{
			Vertex v = queue.poll();
			for(Vertex u : getAdj(v))
			{
				if(distances.get(pairs.get(u)) == -1)
				{
					distances.put(pairs.get(u), distances.get(v)+1);
					queue.add(pairs.get(u));
				}
			}
		}
		
		return (distances.get(nullVertex) != -1);
	}

	private LinkedList<Vertex> getAdj(Vertex v) 
	{
		LinkedList<Vertex> result = new LinkedList<Vertex>();
		for(Edge e : edges)
		{
			if(e.isIncident(v))
				result.add(e.getAdj(v));
		}
		return result;
	}

}
