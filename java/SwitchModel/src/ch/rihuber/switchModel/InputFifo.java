package ch.rihuber.switchModel;

import java.util.LinkedList;

public class InputFifo 
{
	
	LinkedList<Packet> waitingPackets;
	LinkedList<Character> currentPayload;
	
	public InputFifo()
	{
		waitingPackets = new LinkedList<Packet>();
		currentPayload = new LinkedList<Character>();
	}
	
	public void addPacket(Packet newPacket)
	{
		waitingPackets.add(newPacket);
	}
	
	public LinkedList<String> getNextStimulus(String[] lastResponse) throws Exception
	{
		boolean readEnable;
		if(lastResponse == null)
			readEnable = fetchStdLogic("0");
		else
			readEnable = fetchStdLogic(lastResponse[0]);
		
		int payloadByte = 0;
		if(!isEmpty())
		{
			if(currentPayload.size() == 0)
				currentPayload = waitingPackets.poll().getPayload();
			payloadByte = currentPayload.getFirst();
		}
		
		LinkedList<String> result = new LinkedList<String>();
		result.add(toStdLogic(isEmpty()));
		result.add(toStdLogic(isCurrentPayloadEmpty()) + toStdLogicVector(payloadByte, Packet.DATA_WIDTH));
		return result;
	}
	
	private boolean isCurrentPayloadEmpty()
	{
		return (currentPayload.size() == 0);
	}
	
	boolean isEmpty()
	{
		if(waitingPackets.size() > 0)
			return false;
		return isCurrentPayloadEmpty();
	}
	
	private boolean fetchStdLogic(String value) throws Exception
	{
		if(value.equals("1"))
			return true;
		if(value.equals("0"))
			return false;
		
		throw new Exception("Unable to interprete string '" + value + "' as a std_logic");
	}
	
	private String toStdLogic(boolean value)
	{
		return (value) ? "1" : "0";
	}
	
	private String toStdLogicVector(int value, int size) throws Exception
	{
		if(value > (Math.pow(2, size)-1))
			throw new Exception("Unable to represent value " + value + " in a std_logic_vector of length " + size);
		String result = Integer.toString(value, 2);
		for(int i=result.length(); i<size; i++)
		{
			result = "0" + result;
		}
		return result;
	}

}
