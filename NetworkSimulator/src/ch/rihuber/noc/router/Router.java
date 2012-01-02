package ch.rihuber.noc.router;

import ch.rihuber.noc.Node;

public abstract class Router 
{
	
	public abstract String selectForwardingLink(Node switchingNode, Node dest);

}

