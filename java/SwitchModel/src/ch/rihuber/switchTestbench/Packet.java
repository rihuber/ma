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
	private int packetReadyTime = 0, packetArrivalTime = 0, inputFifoIndex, outputFifoIndex;
	private LinkedList<OutputFifo> allowedOutputFifos;
	
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
	
	public static Packet findPacket(LinkedList<Integer> payload, boolean onlyUnreceivedPackets) 
	{
		if(packets == null)
			return null;
		
		for(Packet currentPacket : packets)
		{
			if(onlyUnreceivedPackets)
				if(currentPacket.isCorrectReceived())
					continue;
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
		if(localAddress > 1)
			throw new Exception("Local Address " + localAddress + " out of range (0 to 1)");
		this.id = id;
		this.priority = priority;
		this.localAddress = localAddress;
		this.globalAddress = globalAddress;
		
		createPayload(size);
	}
	
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
		result += "\tPayload: " + payload;
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
				payload.add(255 & (id >> (8*i)));
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
		System.out.println("Starting to transmit packet:");
		System.out.println(toString());
	}

	public void setPacketArrival(int cycle, int fifoIndex) throws Exception 
	{
		packetArrivalTime = cycle;
		outputFifoIndex = fifoIndex;
		if(!isCorrectReceived())
			throw new Exception("Packet not correct received: outputFifo " + outputFifoIndex);
	}

	public static void printReport() 
	{
		String result = "\n\n\n\nTest Report:\n------------\n\n";
		result += "Packets generated: " + packets.size() + "\n";
		
		result += "Packets correclty received: ";
		int i=0;
		for(Packet currentPacket : packets)
		{
			if(currentPacket.isCorrectReceived())
				i++;
		}
		result += i+"\n";
		System.out.println(result);
	}

	private boolean isCorrectReceived() 
	{
		if(packetArrivalTime > 0 && packetArrivalTime > packetReadyTime)
		{
			for(OutputFifo currentOutputFifo : allowedOutputFifos)
				if(currentOutputFifo.getFifoIndex() == outputFifoIndex)
					return true;
		}
		return false;
	}

	public static boolean allPacketsReceived() 
	{
		for(Packet currentPacket : packets)
		{
			if(!currentPacket.isCorrectReceived())
				return false;
		}
		return true;
	}

	public void setAllowedOutputFifos(LinkedList<OutputFifo> allowedOutputFifos) 
	{
		this.allowedOutputFifos = allowedOutputFifos;
	}

	public void addAllowedOutputFifos(OutputFifo outputFifo) 
	{
		if(allowedOutputFifos == null)
			allowedOutputFifos = new LinkedList<OutputFifo>();
		
		allowedOutputFifos.add(outputFifo);
	}

	public static int numPacketsStillToTransmit() 
	{
		int i = 0;
		for(Packet currentPacket : packets)
		{
			if(!currentPacket.isCorrectReceived())
				i++;
		}
		return i;
	}

}
