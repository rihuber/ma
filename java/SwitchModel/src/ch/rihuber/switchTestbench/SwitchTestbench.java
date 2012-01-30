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
	private int numInternalPorts = 2;
	private LinkedList<InputFifo> inputFifos;
	private LinkedList<OutputFifo> outputFifos;
	private LinkedList<OutputFifo> externalOutputFifos;
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
		externalOutputFifos = new LinkedList<OutputFifo>();
		for(int i=0; i<numPorts; i++)
		{
			OutputFifo newOutputFifo = new OutputFifo(i);
			outputFifos.add(newOutputFifo);
			if(i >= numInternalPorts)
				externalOutputFifos.add(newOutputFifo);
		}

		reset = new DefaultReset();
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
			runInitProcedure();
			
			sendOnePacketToExternalPort(20);
			
			runReset();
			sendMultiplePacketsToExternalPort(3, 20);
			
			runReset();
			sendOnePacketToInternalPort(20);
			
			runReset();
			sendMultiplePacketsToInternalPort(3, 20);
			
			
			Packet.printReport();
		} catch (Exception e) {
			throw e;
		} finally {
			closeFiles();
		}
		System.out.println("...done!");
	}
	

	private void sendMultiplePacketsToInternalPort(int numPacketsPerLink, int packetLength) throws Exception 
	{
		for(InputFifo currentInputFifo : inputFifos)
		{
			for(int i=0; i<numPacketsPerLink; i++)
			{
				Packet newPacket = Packet.generatePacket(1, 0, 1, packetLength);
				newPacket.addAllowedOutputFifos(outputFifos.get(0));
				currentInputFifo.addPacket(newPacket);
			}
		}
		runUntilAllPacketsReceived();
	}

	private void sendOnePacketToInternalPort(int packetLength) throws Exception 
	{
		Packet newPacket = Packet.generatePacket(1, 0, 1, packetLength);
		newPacket.addAllowedOutputFifos(outputFifos.get(0));
		inputFifos.getFirst().addPacket(newPacket);
		runUntilAllPacketsReceived();
	}

	private void runInitProcedure() throws Exception
	{
		// fetch initial state
		fetchResponse();
		
		runReset();
	}
	
	private void runReset() throws Exception
	{
		// two reset cycles
		reset.on();
		runCycles(2);
		reset.off();
		
		// two idle cycles
		runCycles(2);
	}
	
	private void sendOnePacketToExternalPort(int packetLength) throws Exception
	{
		Packet newPacket = Packet.generatePacket(1, 1, 2, packetLength);
		newPacket.setAllowedOutputFifos(externalOutputFifos);
		inputFifos.getFirst().addPacket(newPacket);
		runUntilAllPacketsReceived();
	}
	
	private void sendMultiplePacketsToExternalPort(int numPacketsPerLink, int packetLength) throws Exception
	{
		for(InputFifo currentInputFifo : inputFifos)
		{
			for(int i=0; i<numPacketsPerLink; i++)
			{
				Packet newPacket = Packet.generatePacket(1, 1, 2, packetLength);
				newPacket.setAllowedOutputFifos(externalOutputFifos);
				currentInputFifo.addPacket(newPacket);
			}
		}
		runUntilAllPacketsReceived();
	}
	
	private void runUntilAllPacketsReceived() throws Exception 
	{
		while(!Packet.allPacketsReceived())
		{
			runCycles(1);
		}
	}

	private void runCycles(int numCycles) throws Exception
	{
		for(int i=0; i<numCycles; i++)
		{
			applyNextStimulus();
			fetchResponse();
		}
	}

	private void applyNextStimulus() throws Exception 
	{
		System.out.println("Cycle " + cycle++);
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
