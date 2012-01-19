package ch.rihuber.noc.matching;

import java.util.Map.Entry;

public class MatchingResult 
{

	private HopcroftKarp matchingCalculator;

	public MatchingResult(HopcroftKarp matchingCalculator) 
	{
		this.matchingCalculator = matchingCalculator;
	}
	
	public int getLinkWeight()
	{
		return matchingCalculator.getMatchingEdgesCount();
	}

	public String getSelectedMatching() 
	{
		String result = "";
		for(Entry<Vertex, Vertex> currentEntry : matchingCalculator.getPairs().entrySet())
		{
			result += "(" + currentEntry.getKey() + "->" + currentEntry.getValue() + ") ";
		}
		return result;
	}

}
