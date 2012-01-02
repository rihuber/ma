package ch.rihuber.noc;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import ch.rihuber.noc.matching.MatchingResult;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.Topology;


public class NetworkSimulator 
{
	private final int GRID_SIZE = 5;
	private final String outputFilename = "gridXY5";

	public static void main(String[] args) 
	{
		new NetworkSimulator().run();
	}

	private void run() 
	{
		
		Topology topology = new GridTopology(GRID_SIZE, GridTopology.XY_ROUTING);
		
		sendExplorerPackages(topology);
		
		printResults(topology);
		
		System.out.println("...done!");
	}

	private void printResults(Topology topology) 
	{
		String output = topology.toString();
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
		
		try {
			FileOutputStream outputStream = new FileOutputStream(outputFilename);
			outputStream.write(output.getBytes());
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
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
	
}