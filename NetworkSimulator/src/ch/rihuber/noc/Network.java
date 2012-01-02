package ch.rihuber.noc;

import ch.rihuber.noc.load.Load;
import ch.rihuber.noc.load.LoadIterator;
import ch.rihuber.noc.topology.Topology;

public class Network 
{
	
	private Topology topology;
	
	private LoadIterator loadIterator;
	
	private Load currentLoad;
	
	public Network(Topology topology)
	{
		this.topology = topology;
		loadIterator = new LoadIterator(topology.getNodes());
	}

	public boolean hasNextLoad()
	{
		return loadIterator.hasNext();
	}
	
	public void nextLoad()
	{
		topology.clearLoads();
		currentLoad = new Load(loadIterator.next(), 1);
		currentLoad.apply();
		topology.routeCurrentLoads();
	}

	public Link getBottleneckLink() 
	{
		return topology.getBottleneckLink();
	}

	public Load getCurrentLoad() 
	{
		return currentLoad;
	}
	
}
