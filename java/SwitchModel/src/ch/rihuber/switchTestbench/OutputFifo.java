package ch.rihuber.switchTestbench;

import java.util.LinkedList;
import java.util.List;

import ch.rihuber.vhdl.Model;
import ch.rihuber.vhdl.StdLogic;
import ch.rihuber.vhdl.StdLogicVector;
import ch.rihuber.vhdl.VhdlDataType;

public class OutputFifo implements Model
{
	
	private LinkedList<Packet> receivedPackets;
	private LinkedList<Integer> currentPacket;
	private int cycle = 0;
	private int fifoIndex;
	
	private boolean full;
	
	public OutputFifo(int fifoIndex)
	{
		this.fifoIndex = fifoIndex;
		currentPacket = new LinkedList<Integer>();
		receivedPackets = new LinkedList<Packet>();
		full = false;
	}
	
	@Override
	public void applyResponse(List<String> response) throws Exception
	{
		cycle++;
		StdLogic writeEnable = StdLogic.create(response.get(0));
		StdLogicVector data = new StdLogicVector(response.get(1));
		
		if(writeEnable.toBoolean())
		{
			int payload = data.subVector(7, 0).toInteger();
			currentPacket.add(payload);
			
			StdLogic endOfPacket = data.valueAt(8);
			if(endOfPacket.toBoolean())
			{
				LinkedList<Integer> payloadCopy = new LinkedList<Integer>(currentPacket);
				Packet completedPacket = Packet.findPacket(payloadCopy);
				if(completedPacket == null)
					System.out.println("Unable to find packet. Packet broken on transmission?");
				else
				{
					completedPacket.setPacketArrival(cycle, fifoIndex);
					receivedPackets.add(completedPacket);
					System.out.println("Received packet completely:\n"+completedPacket);
				}
				currentPacket = new LinkedList<Integer>();
			}
		}
	}
	
	public LinkedList<VhdlDataType> getNextStimulus()
	{
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		result.add(StdLogic.ZERO); //full
		return result;
	}

}
