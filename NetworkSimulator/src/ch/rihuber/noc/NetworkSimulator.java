package ch.rihuber.noc;
import ch.rihuber.noc.matching.MatchingResult;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;


public class NetworkSimulator 
{

	private final int GRID_SIZE = 4;

	public static void main(String[] args) 
	{
		new NetworkSimulator().run();
	}

	private void run() 
	{
		
		Topology topology = new GridTopology(GRID_SIZE, GridTopology.XY_ROUTING);
		
		sendExplorerPackages(topology);
		
		printResults(topology);
	}

	private void printResults(Topology topology) 
	{
		String output = "";
		int totalWeight = 0;
		for(Link link : topology.getLinks())
		{
			MatchingResult matchingResult = link.getMatchingResult();
			
			totalWeight += matchingResult.getLinkWeight();
			
			output += link + ":\n";
			output += "Weight: " + matchingResult.getLinkWeight() + "\n";
			output += "Selected matching: " + matchingResult.getSelectedMatching() + "\n";
			output += "\n";			
		}
		
		output += "\n\n-----------------------";
		output += "\nTotal link weight: " + totalWeight;
		System.out.println(output);
	}

	private void sendExplorerPackages(Topology topology) 
	{
		for(Node sourceNode : topology.getNodes())
		{
			for(Node destNode : topology.getNodes())
			{
				sourceNode.forward(sourceNode, destNode);
			}
		}
	}
	
	private void printSummary(Topology topology) 
	{
		for(Link currentLink : topology.getLinks())
		{
			System.out.println(currentLink);
		}
	}
}