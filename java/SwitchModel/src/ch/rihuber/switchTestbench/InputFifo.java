package ch.rihuber.switchTestbench;

import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import sun.text.normalizer.UProperty;

import com.sun.xml.internal.ws.message.source.PayloadSourceMessage;

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
	private boolean firstByteOfPacket;
	private boolean isFirstByteOfPacket;
	private boolean emptyPhase;
	
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
		
		updateState();
		
		LinkedList<VhdlDataType> result = createStimulus();
		
		
//		// Random empty cycles
//		Random rand = new Random();
//		if(rand.nextInt(2) == 0)
//		{
//			empty = StdLogic.ONE;
//			result.add(empty);
//			result.add(data);
//			return result;
//		}
		
		return result;
	}
	
	private void updateState()
	{
		if(packetTransferInProgress())
		{
			if(readEnable == StdLogic.ONE && !isEmptyPhase())
			{
				currentPayload.poll();
				if(currentPayload.size() == 0)
					currentPayload = null;
			}
		}
		
		if(!packetTransferInProgress())
		{
			if(waitingPackets.size() > 0)
			{
				Packet nextPacket = waitingPackets.poll();
				nextPacket.setPacketReady(cycle, fifoIndex);
				currentPayload = nextPacket.getPayload();
				isFirstByteOfPacket = true;
			}
				
		}
		
		Random rand = new Random();
		if(isEmptyPhase())
		{
			if(rand.nextInt(10) > 7)
				emptyPhase = false;
		}
		else
		{
			if(!firstByteOfPacket)
			{
				if(rand.nextInt(10) > 2)
					emptyPhase = true;
			}
		}
	}
	
	private LinkedList<VhdlDataType> createStimulus() throws Exception 
	{
		if(!packetTransferInProgress())
			return createEmptyStimulus();
		
		if(isEmptyPhase())
			return createEmptyStimulus();
		
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
			
		int currentPayloadByte = currentPayload.getFirst();
		StdLogic empty = StdLogic.ZERO;
		StdLogic endOfPacket = (currentPayload.size() > 1) ? StdLogic.ZERO : StdLogic.ONE;
		StdLogicVector data = new StdLogicVector(currentPayloadByte, 9);
		data.setValueAt(8, endOfPacket);
		
		result.add(empty);
		result.add(data);
		return result;
	}

	private boolean isEmptyPhase() 
	{
		return emptyPhase;
	}

	private LinkedList<VhdlDataType> createEmptyStimulus() 
	{
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		
		StdLogic empty = StdLogic.ONE;
		StdLogic endOfPacket = StdLogic.DONT_CARE;
		StdLogicVector data = new StdLogicVector(StdLogic.DONT_CARE, 9);
		data.setValueAt(8, endOfPacket);
		
		result.add(empty);
		result.add(data);
		return result;
	}

	private boolean packetTransferInProgress() 
	{
		return currentPayload != null;
	}

	@Override
	public void applyResponse(List<String> response) throws Exception 
	{
		readEnable = StdLogic.create(response.get(0));
//		System.out.println("inputFifo " + fifoIndex+" readEnable: " + readEnable.toString());
	}
	
//	boolean isEmpty()
//	{
//		if(currentPayload != null && currentPayload.size() > 0)
//			return false;
//		
//		return true;
//	}

}
