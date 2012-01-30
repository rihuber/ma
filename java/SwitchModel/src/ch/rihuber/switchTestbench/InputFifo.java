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
		
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		
		StdLogic empty = StdLogic.ONE;
		StdLogic endOfPacket = StdLogic.DONT_CARE;
		StdLogicVector data = new StdLogicVector(StdLogic.DONT_CARE, 9);
		
		if(currentPayload != null && readEnable == StdLogic.ONE)
			currentPayload.poll();
		
		if(currentPayload == null || currentPayload.size() == 0)
		{
			Packet nextPacket = waitingPackets.poll();
			if(nextPacket == null)
				currentPayload = null;
			else
			{
				nextPacket.setPacketReady(cycle, fifoIndex);
				currentPayload = nextPacket.getPayload();
			}
		}
		
		if(currentPayload != null)
		{
			empty = StdLogic.ZERO;
			int nextPayloadByte = currentPayload.getFirst();
			data = new StdLogicVector(nextPayloadByte, 9);
			endOfPacket = (currentPayload.size() == 1) ? StdLogic.ONE : StdLogic.ZERO;
		}
		data.setValueAt(8, endOfPacket);
		result.add(empty);
		result.add(data);

//		System.out.println("Fifo "+fifoIndex+" writes stimulus "+data.toString());
		return result;
	}
	
	@Override
	public void applyResponse(List<String> response) throws Exception 
	{
		readEnable = StdLogic.create(response.get(0));
//		System.out.println("inputFifo " + fifoIndex+" readEnable: " + readEnable.toString());
	}
	
	boolean isEmpty()
	{
		if(currentPayload != null && currentPayload.size() > 0)
			return false;
		
		return true;
	}

}
