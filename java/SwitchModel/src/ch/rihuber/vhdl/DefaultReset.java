package ch.rihuber.vhdl;

import java.util.LinkedList;
import java.util.List;

public class DefaultReset implements IVhdlModel 
{
	
	private boolean doReset = true;
	
	@Override
	public void applyResponse(List<String> response) throws Exception 
	{
		throw new Exception("No response expected!");
	}

	@Override
	public LinkedList<VhdlDataType> getNextStimulus() throws Exception 
	{
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		
		StdLogic reset = (doReset) ? StdLogic.ZERO:StdLogic.ONE;
		result.add(reset);
		
		return result;
	}

	public void off() 
	{
		doReset = false;
	}
	
	public void on()
	{
		doReset = true;
	}

}
