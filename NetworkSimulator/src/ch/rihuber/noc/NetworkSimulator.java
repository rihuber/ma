package ch.rihuber.noc;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import ch.rihuber.noc.matching.MatchingResult;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.RingTopology;
import ch.rihuber.noc.topology.Topology;


public class NetworkSimulator 
{
	
	private final static String GRID_TOPOLOGY = "grid";
	private static final Object RING_TOPOLOGY = "ring";
	
	private final static String TOPOLOGY_FLAG = "-t";
	private static final String NODE_COUNT_FLAG = "-s";
	private static final String FILE_NAME_FLAG = "-f";
	
	
	public static void main(String[] args) 
	{
		String selectedTopology = GRID_TOPOLOGY;
		int selectedNodeCount = 1;
		String selectedOutputFileName = null;
		
		for(int i=0; i<args.length; i++)
		{
			if(args[i].equals(TOPOLOGY_FLAG))
			{
				selectedTopology = args[++i];
				continue;
			}
			if(args[i].equals(NODE_COUNT_FLAG))
			{
				selectedNodeCount = Integer.parseInt(args[++i]);
				continue;
			}
			if(args[i].equals(FILE_NAME_FLAG))
			{
				selectedOutputFileName = args[++i];
				continue;
			}
		}
		new NetworkSimulator().run(selectedTopology, selectedNodeCount, selectedOutputFileName);
	}

	private void run(String topologyName, int nodeCount, String outputFileName) 
	{
		Topology topology = null;
		if(topologyName.equals(GRID_TOPOLOGY))
			topology = new GridTopology(nodeCount);
		if(topologyName.equals(RING_TOPOLOGY))
			topology = new RingTopology(nodeCount);
//		System.out.println(topology);
		
		sendExplorerPackages(topology);
		
		printResults(topology, outputFileName);
		
		System.out.println("...done!");
	}

	private void printResults(Topology topology, String outputFileName) 
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
		output += "\n-----------------------\n";
		
		if(outputFileName == null)
		{
			System.out.println(output);
			return;
		}
		try {
			FileOutputStream outputStream = new FileOutputStream(outputFileName);
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