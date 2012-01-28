package ch.rihuber.switchModel;

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
	private LinkedList<Character> payload;
	
	public Packet(int id, int priority, int localAddress, int globalAddress, int size) throws Exception
	{
		this.id = id;
		this.priority = priority;
		this.localAddress = localAddress;
		this.globalAddress = globalAddress;
		
		createRandomPayload(size);
	}
	
	public Packet(LinkedList<Character> payload)
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
		return result;
	}
	
	public LinkedList<Character> getPayload()
	{
		LinkedList<Character> payloadClone = new LinkedList<Character>(payload);
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
			char currentByte = payload.get(i+1);
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
		
		char headerByte = payload.getFirst();
		this.globalAddress = headerByte & getGlobalAddressMask();
		this.localAddress = (headerByte & getLocalAddressMask()) >> GLOBAL_ADDRESS_WIDTH;
		this.priority = (headerByte & getPriorityMask()) >> (ADDRESS_WIDTH);
	}

	private void createRandomPayload(int size) throws Exception
	{
		if(size < 1)
			throw new Exception("Unable to create packet with size " + size);
		
		payload = new LinkedList<Character>();
		payload.add(createHeader());
		
		if(size <= 4)
			return;
		
		for(int i=3; i>=0; i--)
			payload.add((char)(id >> (8*i)));
		
		Random rand = new Random();
		for(int i=6; i<size; i++)
			payload.add((char)rand.nextInt());
	}

	private char createHeader() 
	{
		int header = globalAddress;
		header = header | (localAddress << GLOBAL_ADDRESS_WIDTH);
		header = header | (priority << (GLOBAL_ADDRESS_WIDTH + LOCAL_ADDRESS_WIDTH));
		return (char)header;
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
