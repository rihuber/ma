package ch.rihuber.noc.load;

import java.util.LinkedList;

import ch.rihuber.noc.Node;

public class LoadIterator 
{
	
	private LinkedList<Node> availableNodes, subNodes;
	private Node currentNode;
	private int currentIndex;
	private LoadIterator currentSubIterator;

	public LoadIterator(LinkedList<Node> availableNodes)
	{
		this.availableNodes = availableNodes;
		currentIndex = 0;
		
		if(availableNodes.size() > 1)
		{
			subNodes = new LinkedList<Node>(availableNodes);
			currentNode = subNodes.remove(currentIndex);
			currentSubIterator = new LoadIterator(subNodes);
		}
	}
	

	public boolean hasNext()
	{
		if(availableNodes.size() == 1)
		{
			return (currentIndex == 0);
		}
		
		if(currentIndex+1 < availableNodes.size())
			return true;
		if(currentSubIterator.hasNext())
			return true;
		return false;
	}
	
	public LinkedList<Node> next()
	{
		if(availableNodes.size() == 1)
		{
			currentIndex++;
			return new LinkedList<Node>(availableNodes);
		}

		if(!currentSubIterator.hasNext())
		{
			subNodes.add(currentIndex, currentNode);
			currentIndex++;
			currentNode = subNodes.remove(currentIndex);
			currentSubIterator = new LoadIterator(subNodes);
			if(availableNodes.size() > 10)
			{
				System.out.println("Iteration " + currentIndex + " of " + availableNodes.size());
			}
		}
		LinkedList<Node> result = currentSubIterator.next();
		result.push(currentNode);
		return result;
	}

}
