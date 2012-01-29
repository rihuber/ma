package ch.rihuber.switchTestbench;

import java.io.*;
import java.util.LinkedList;

import ch.rihuber.vhdl.DefaultReset;
import ch.rihuber.vhdl.VhdlDataType;

public class SwitchTestbench 
{
	
	private BufferedReader in = null;
	private PrintWriter out = null;
	
	private String responseFileName;
	private String stimuliFileName;
	
	private int numPorts = 6;
	private LinkedList<InputFifo> inputFifos;
	private LinkedList<OutputFifo> outputFifos;
	private DefaultReset reset;
	
	private int cycle = 0;
	
	public SwitchTestbench(String stimuliFileName, String responseFileName) throws FileNotFoundException 
	{
		this.stimuliFileName = stimuliFileName;
		this.responseFileName = responseFileName;
		
		inputFifos = new LinkedList<InputFifo>();
		for(int i=0; i<numPorts; i++)
			inputFifos.add(new InputFifo(i));
		
		outputFifos = new LinkedList<OutputFifo>();
		for(int i=0; i<numPorts; i++)
			outputFifos.add(new OutputFifo(i));
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
		
		new SwitchTestbench(stimuliFileName, responseFileName).run();
	}
	
	private void run() throws Exception 
	{
		try 
		{
			// reset cycle
			reset = new DefaultReset();
			applyNextStimulus();
			fetchResponse();
			for(InputFifo currentInputFifo : inputFifos)
			{
				currentInputFifo.addPacket(Packet.generatePacket(1, 1, 2, 20));
			}
			for(int i=0; i<300; i++)//while(!allFifosEmpty())
			{
				applyNextStimulus();
				fetchResponse();
			}
		} catch (Exception e) {
			throw e;
		} finally {
			closeFiles();
		}
		System.out.println("...done!");
	}

	private void applyNextStimulus() throws Exception 
	{
		if(out == null)
			createStimuliWriter();

		LinkedList<VhdlDataType> nextStimulus = new LinkedList<VhdlDataType>();
		
		nextStimulus.addAll(reset.getNextStimulus());
		
		for(InputFifo currentInputFifo : inputFifos)
			nextStimulus.addAll(currentInputFifo.getNextStimulus());
		
		for(OutputFifo currentOutputFifo : outputFifos)
			nextStimulus.addAll(currentOutputFifo.getNextStimulus());
		
		String outLine = "";
		for(VhdlDataType currentSignal : nextStimulus)
		{
			outLine += " " + currentSignal.toString();
		}
		
		out.write(outLine+"\n");
		out.flush();
		//System.out.println("Written stimulus: "+outLine);
	}
	
	private void fetchResponse() throws Exception 
	{
		if(in == null)
			createResponseReader();
		
		String inLine = in.readLine();
		if(inLine == null)
			throw new Exception("Unable to read response line.");
		//System.out.println("Fetched response: "+inLine);
		
		String[] responseElements = inLine.split("\\s");
		LinkedList<String> response = new LinkedList<String>();
		for(int i=0; i<responseElements.length; i++)
			response.add(responseElements[i]);
		
		int i = 0;
		for(InputFifo currentInputFifo : inputFifos)
		{
			currentInputFifo.applyResponse(response.subList(i, i+1)); // read enable
			i++;
		}
		for(OutputFifo currentOutputFifo : outputFifos)
		{
			currentOutputFifo.applyResponse(response.subList(i, i+2)); // write enable, data
			i += 2;
		}
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
