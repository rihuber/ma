package ch.rihuber.noc;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import ch.rihuber.noc.matching.MatchingResult;
import ch.rihuber.noc.topology.GridTopology;
import ch.rihuber.noc.topology.RingTopology;
import ch.rihuber.noc.topology.SpinTopology;
import ch.rihuber.noc.topology.Topology;
import ch.rihuber.noc.topology.TorusTopology;


public class NetworkSimulator 
{
	
	private final static String GRID_TOPOLOGY = "grid";
	private final static String RING_TOPOLOGY = "ring";
	private final static String TORUS_TOPOLOGY = "torus";
	private final static String SPIN_TOPOLOGY = "spin";
	
	private final static String TOPOLOGY_FLAG = "-t";
	private final static String NODE_COUNT_FLAG = "-s";
	private final static String FILE_NAME_FLAG = "-f";
	private final static String ROUTER_FLAG = "-r";
	private final static String FANOUT_FLAG = "--fanout";
	
	
	public static void main(String[] args) 
	{
		String selectedTopology = GRID_TOPOLOGY;
		int selectedNodeCount = 1;
		String selectedOutputFileName = null;
		String selectedRouter = null;
		int selectedFanout = 2;
		
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
			if(args[i].equals(ROUTER_FLAG))
			{
				selectedRouter = args[++i];
				continue;
			}
			if(args[i].equals(FANOUT_FLAG))
			{
				selectedFanout = Integer.parseInt(args[++i]);
				continue;
			}
		}
		new NetworkSimulator().run(selectedTopology, selectedNodeCount, selectedOutputFileName, selectedRouter, selectedFanout);
	}

	private void run(String topologyName, int nodeCount, String outputFileName, String routerName, int fanout) 
	{
		Topology topology = null;
		if(topologyName.equals(GRID_TOPOLOGY))
			topology = new GridTopology(nodeCount, routerName);
		if(topologyName.equals(RING_TOPOLOGY))
			topology = new RingTopology(nodeCount, routerName);
		if(topologyName.equals(TORUS_TOPOLOGY))
			topology = new TorusTopology(nodeCount, routerName);
		if(topologyName.equals(SPIN_TOPOLOGY))
			topology = new SpinTopology(nodeCount, fanout, routerName);
//		System.out.println(topology);
		
		topology.sendExplorerPackages();
		
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

}