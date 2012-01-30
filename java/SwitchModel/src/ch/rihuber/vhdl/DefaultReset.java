package ch.rihuber.vhdl;

import java.util.LinkedList;
import java.util.List;

public class DefaultReset implements IVhdlModel 
{
	
	boolean firstInvocation = true;
	
	@Override
	public void applyResponse(List<String> response) throws Exception 
	{
		throw new Exception("No response expected!");
	}

	@Override
	public LinkedList<VhdlDataType> getNextStimulus() throws Exception 
	{
		LinkedList<VhdlDataType> result = new LinkedList<VhdlDataType>();
		
		if(firstInvocation)
			result.add(StdLogic.ZERO);
		else
			result.add(StdLogic.ONE);
		
		firstInvocation = false;
		
		return result;
	}

}
