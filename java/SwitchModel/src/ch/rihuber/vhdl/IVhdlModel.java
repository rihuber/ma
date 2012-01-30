package ch.rihuber.vhdl;

import java.util.LinkedList;
import java.util.List;

public interface IVhdlModel 
{
	public void applyResponse(List<String> response) throws Exception;
	public LinkedList<VhdlDataType> getNextStimulus() throws Exception;
}
