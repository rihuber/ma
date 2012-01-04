package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;
import ch.rihuber.noc.topology.RingTopology;

public class SimpleRingRouter extends Router 
{

	@Override
	public String selectForwardingLink(Node switchingNode, Node dest) 
	{
		return RingTopology.FORWARD;
	}
	
	public String toString()
	{
		return "Simple Ring Router";
	}

}
