package ch.rihuber.switchTestbench;

import java.util.LinkedList;
import java.util.List;

import ch.rihuber.vhdl.IVhdlModel;
import ch.rihuber.vhdl.StdLogic;
import ch.rihuber.vhdl.StdLogicVector;
import ch.rihuber.vhdl.VhdlDataType;

public class InputFifo implements IVhdlModel
{
	
	private LinkedList<Packet> waitingPackets;
	private LinkedList<Integer> currentPayload;
	private int cycle = 0;
	private int fifoIndex;
	
	private StdLogic readEnable;
	
	public InputFifo(int fifoIndex)
	{
		this.fifoIndex = fifoIndex;
		waitingPackets = new LinkedList<Packet>();
		currentPayload = null;
		readEnable = StdLogic.ZERO;
	}
	
	public void addPacket(Packet newPacket)
	{
		waitingPackets.add(newPacket);
	}
	
	@Override
	public LinkedList<VhdlDataType> getNextStimulus() throws Exception
	{
		cycle++;
		fetchWaitingPacket();
		
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		result.add(StdLogic.create(isEmpty()));
		
		StdLogicVector data;
		if(!isEmpty())
		{
			if(readEnable.toBoolean())
				currentPayload.poll();
			
			data = new StdLogicVector(currentPayload.getFirst(), 9);

			if(currentPayload.size() == 1)
			{
				data.setValueAt(8, StdLogic.ONE);
				if(readEnable == StdLogic.ONE)
					currentPayload = null;
			}
			else
				data.setValueAt(8, StdLogic.ZERO);
		}
		else
		{
			data = new StdLogicVector(StdLogic.DONT_CARE, 9);
		}
		result.add(data);
		
		return result;
	}
	
	private void fetchWaitingPacket()
	{
		if(currentPayload == null)
		{
			Packet nextPacket = waitingPackets.poll();
			if(nextPacket != null)
			{
				nextPacket.setPacketReady(cycle, fifoIndex);
				System.out.println("Starting to transmit a new packet:\n" + nextPacket);
				currentPayload = nextPacket.getPayload();
			}
		}
	}
	
	@Override
	public void applyResponse(List<String> response) throws Exception 
	{
		readEnable = StdLogic.create(response.get(0));
	}
	
	boolean isEmpty()
	{
		if(waitingPackets.size() > 0)
			return false;
		
		if(currentPayload != null)
			return false;
		
		return true;
	}

}
