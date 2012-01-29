package ch.rihuber.switchTestbench;

import java.util.LinkedList;
import java.util.Random;

public class Packet 
{
	public final static int DATA_WIDTH = 8;
	public final static int ADDRESS_WIDTH = 6;
	public final static int LOCAL_ADDRESS_WIDTH = 2;
	public final static int GLOBAL_ADDRESS_WIDTH = ADDRESS_WIDTH-LOCAL_ADDRESS_WIDTH;
	public static final int PRIORITY_WIDTH = DATA_WIDTH - ADDRESS_WIDTH;
	
	private int id, priority, localAddress, globalAddress;
	private LinkedList<Integer> payload;
	
	public Packet(int id, int priority, int localAddress, int globalAddress, int size) throws Exception
	{
		this.id = id;
		this.priority = priority;
		this.localAddress = localAddress;
		this.globalAddress = globalAddress;
		
		createPayload(size);
	}
	
	public Packet(LinkedList<Integer> payload)
	{
		this.payload = payload;
		extractHeader();
		extractId();
	}
	
	public String toString()
	{
		String result = "Packet " + id + "\n";
		result += "\tGlobalAddress: " + globalAddress + "\n";
		result += "\tLocalAddress: " + localAddress + "\n";
		result += "\tPriority: " + priority + "\n";
		result += "\tOverall packet size: " + payload.size() + "\n";
		result += "\tPayload:\n";
		for(int i=0; i<payload.size(); i++)
			result += "\t\t" + Integer.toString(payload.get(i), 2) + "\n";
		return result;
	}
	
	public LinkedList<Integer> getPayload()
	{
		LinkedList<Integer> payloadClone = new LinkedList<Integer>(payload);
		return payloadClone;
	}

	private void extractId() 
	{
		if(payload.size() < 5)
		{
			id = 0;
			return;
		}
		for(int i=0; i<4; i++)
		{
			int currentByte = payload.get(i+1);
			id = id | (currentByte << ((3-i)*8));
		}
	}

	private void extractHeader() 
	{
		if(payload.size() < 1)
		{
			globalAddress = 0;
			localAddress = 0;
			priority = 0;
			return;
		}
		
		int headerByte = payload.getFirst();
		this.globalAddress = headerByte & getGlobalAddressMask();
		this.localAddress = (headerByte & getLocalAddressMask()) >> GLOBAL_ADDRESS_WIDTH;
		this.priority = (headerByte & getPriorityMask()) >> (ADDRESS_WIDTH);
	}

	private void createPayload(int size) throws Exception
	{
		int currentByte = 0;
		payload = new LinkedList<Integer>();
		
		// the header is required, since it contains the routing information
		if(currentByte++ < size)
			payload.add(createHeader());
		else
			throw new Exception("Packet size " + size + " is not valid");
		
		// add the id to the packet if there is enough space
		if(size-currentByte >= 4)
			for(int i=3; i>=0; i--)
				payload.add(id >> (8*i));
		
		// fill the rest of the packet with random data
		Random rand = new Random();
		while(currentByte < size)
			payload.add(rand.nextInt(255));
	}

	private int createHeader() 
	{
		int header = globalAddress;
		header = header | (localAddress << GLOBAL_ADDRESS_WIDTH);
		header = header | (priority << (GLOBAL_ADDRESS_WIDTH + LOCAL_ADDRESS_WIDTH));
		return header;
	}
	
	private static int getGlobalAddressMask()
	{
		int mask = 0;
		for(int i=0; i<GLOBAL_ADDRESS_WIDTH; i++)
			mask = mask | (1<<i);
		return mask;
	}
	
	private static int getLocalAddressMask()
	{
		int mask = 0;
		for(int i=GLOBAL_ADDRESS_WIDTH; i<ADDRESS_WIDTH; i++)
			mask = mask | (1<<i);
		return mask;
	}
	
	private static int getPriorityMask()
	{
		int mask = 0;
		for(int i=ADDRESS_WIDTH; i<ADDRESS_WIDTH + PRIORITY_WIDTH; i++)
			mask = mask | (1<<i);
		return mask;
	}
	
}
