package ch.rihuber.vhdl;

import java.util.LinkedList;
import java.util.List;

public interface Model 
{
	public void applyResponse(List<String> response) throws Exception;
	public LinkedList<VhdlDataType> getNextStimulus() throws Exception;
}
