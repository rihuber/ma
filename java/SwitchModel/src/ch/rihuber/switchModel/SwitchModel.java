package ch.rihuber.switchModel;

import java.io.*;
import java.util.LinkedList;

public class SwitchModel 
{
	
	private BufferedReader in = null;
	private PrintWriter out = null;
	private boolean hasNextStimulus = true;
	private String responseFileName;
	private String stimuliFileName;
	
	private int numPorts = 6;
	private LinkedList<InputFifo> inputFifos;
	
	private String reset = "0";
	private String[] lastResponse;
	
	public SwitchModel(String stimuliFileName, String responseFileName) throws FileNotFoundException 
	{
		this.stimuliFileName = stimuliFileName;
		this.responseFileName = responseFileName;
		
		inputFifos = new LinkedList<InputFifo>();
		for(int i=0; i<numPorts; i++)
			inputFifos.add(new InputFifo());
	}
	
	public static void main(String[] args) throws Exception 
	{
		String stimuliFileName = null, responseFileName = null;
		for(int i=0; i<args.length; i++)
		{
			if(args[i].startsWith("stimuliFileName="))
				stimuliFileName = args[i].substring(16);
			if(args[i].startsWith("responseFileName="))
				responseFileName = args[i].substring(17);
		}
		if(stimuliFileName == null)
			throw new Exception("No input file specified");
		if(responseFileName == null)
			throw new Exception("No response file specified");
		
		new SwitchModel(stimuliFileName, responseFileName).run();
	}
	
	private void run() throws Exception 
	{
		try 
		{
			applyNextStimulus();
			lastResponse = fetchResponse();
			reset = "1";
			inputFifos.getFirst().addPacket(new Packet(1, 1, 1, 2, 20));
			for(int i=0; i<3; i++)//while(!allFifosEmpty())
			{
				applyNextStimulus();
				lastResponse = fetchResponse();
			}
		} catch (Exception e) {
			throw e;
		} finally {
			closeFiles();
		}
	}
	
	private boolean allFifosEmpty() 
	{
		for(InputFifo currentInputFifo : inputFifos)
		{
			if(!currentInputFifo.isEmpty())
				return false;
		}
		return true;
	}

	private void applyNextStimulus() throws Exception 
	{
		if(out == null)
			createStimuliWriter();

		LinkedList<String> nextStimulus = new LinkedList<String>();
		nextStimulus.add(reset);
		for(int i=0; i<inputFifos.size(); i++)
		{
			InputFifo currentInputFifo = inputFifos.get(i);
			String[] correspondingResponseEntries = null;
			if(lastResponse != null)
			{
				correspondingResponseEntries = new String[1];
				correspondingResponseEntries[0] = lastResponse[i*3];
			}
			nextStimulus.addAll(currentInputFifo.getNextStimulus(correspondingResponseEntries));
		}
		for(int i=0; i<6; i++) // for all output ports
		{
			nextStimulus.add("0"); // full
		}
		String outLine = nextStimulus.get(0);
		for(int i=1; i<nextStimulus.size(); i++)
			outLine += " " + nextStimulus.get(i);
		
		out.write(outLine+"\n");
		out.flush();
		System.out.println("Written stimulus: "+outLine);
	}
	
	private String[] fetchResponse() throws Exception 
	{
		if(in == null)
			createResponseReader();
		
		String inLine = in.readLine();
		if(inLine == null)
			throw new Exception("Unable to read response line.");
		System.out.println("Fetched response: "+inLine);
		return inLine.split("\\s");
	}
	
	private void createStimuliWriter() throws FileNotFoundException, UnsupportedEncodingException 
	{
		FileOutputStream fileOutputStream = new FileOutputStream(stimuliFileName);
		OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream, "US-ASCII");
		out = new PrintWriter(outputStreamWriter);
		System.out.println("Opened stimuli file");
	}
	
	private void createResponseReader() throws FileNotFoundException 
	{
		in = new BufferedReader(new InputStreamReader(new FileInputStream(responseFileName)));
		System.out.println("Opened response file");
	}
	
	private void closeFiles() throws IOException 
	{
		if(out!=null)
			out.close();
		if(in!=null)
			in.close();
	}
}
