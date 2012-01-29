package ch.rihuber.switchTestbench;

import java.util.Iterator;
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
	private int packetReadyTime, packetArrivalTime, inputFifoIndex, outputFifoIndex;
	
	private static int nextId = 1;
	private static LinkedList<Packet> packets;
	
	public static Packet generatePacket(int priority, int localAddress, int globalAddress, int size) throws Exception
	{
		Packet newPacket = new Packet(nextId++, priority, localAddress, globalAddress, size);
		
		if(packets == null)
			packets = new LinkedList<Packet>();
		
		packets.add(newPacket);
		return newPacket;
	}
	
	public static Packet findPacket(LinkedList<Integer> payload) 
	{
		for(Packet currentPacket : packets)
		{
			if(currentPacket.isPayloadEqual(payload))
				return currentPacket;
		}
		return null;
	}
	
	private boolean isPayloadEqual(LinkedList<Integer> payloadToCompare)
	{
		if(payload.size() != payloadToCompare.size())
			return false;
		
		Iterator<Integer> payloadToCompareIterator = payloadToCompare.iterator();
		for(Integer nextByte : payload)
		{
			if(!payloadToCompareIterator.next().equals(nextByte))
				return false;
		}
		
		return true;
	}
	
	private Packet(int id, int priority, int localAddress, int globalAddress, int size) throws Exception
	{
		this.id = id;
		this.priority = priority;
		this.localAddress = localAddress;
		this.globalAddress = globalAddress;
		
		createPayload(size);
	}
	
//	public Packet(LinkedList<Integer> payload)
//	{
//		this.payload = payload;
//		extractHeader();
//		extractId();
//	}
	
	public String toString()
	{
		String result = "Packet " + id + "\n";
		result += "\tGlobalAddress: " + globalAddress + "\n";
		result += "\tLocalAddress: " + localAddress + "\n";
		result += "\tPriority: " + priority + "\n";
		result += "\tOverall packet size: " + payload.size() + "\n";
		if(packetReadyTime != 0)
			result += "\tReady to transmit in cycle " + packetReadyTime + " at input fifo " + inputFifoIndex + "\n";
		if(packetArrivalTime != 0)
			result += "\tTransmission completed in cycle " + packetArrivalTime + " at output fifo " + outputFifoIndex + "\n";
//		result += "\tPayload:\n";
//		for(int i=0; i<payload.size(); i++)
//			result += "\t\t" + Integer.toString(payload.get(i), 2) + "\n";
		return result;
	}
	
	public LinkedList<Integer> getPayload()
	{
		LinkedList<Integer> payloadClone = new LinkedList<Integer>(payload);
		return payloadClone;
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
		{
			for(int i=3; i>=0; i--)
				payload.add(id >> (8*i));
			currentByte += 4;
		}
		
		// fill the rest of the packet with random data
		Random rand = new Random();
		while(currentByte++ < size)
			payload.add(rand.nextInt(255));
	}

	private int createHeader() 
	{
		int header = globalAddress;
		header = header | (localAddress << GLOBAL_ADDRESS_WIDTH);
		header = header | (priority << (GLOBAL_ADDRESS_WIDTH + LOCAL_ADDRESS_WIDTH));
		return header;
	}
	
	public void setPacketReady(int cycle, int fifoIndex) 
	{
		packetReadyTime = cycle;
		inputFifoIndex = fifoIndex;
	}

	public void setPacketArrival(int cycle, int fifoIndex) 
	{
		packetArrivalTime = cycle;
		outputFifoIndex = fifoIndex;
	}

}
