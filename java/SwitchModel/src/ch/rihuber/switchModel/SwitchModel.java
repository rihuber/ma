package ch.rihuber.switchModel;

import java.io.*;

public class SwitchModel 
{
	
	private BufferedReader in = null;
	private PrintWriter out = null;
	private boolean hasNextStimulus = true;
	private int cycles = 0;
	private String responseFileName;
	private String stimuliFileName;
	
	public SwitchModel(String stimuliFileName, String responseFileName) throws FileNotFoundException 
	{
		this.stimuliFileName = stimuliFileName;
		this.responseFileName = responseFileName;
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
			while(hasNextStimulus)
			{
				applyNextStimulus();
				fetchResponse();
			}
		} catch (Exception e) {
			throw e;
		} finally {
			closeFiles();
		}
	}
	
	private void applyNextStimulus() throws FileNotFoundException 
	{
		if(out == null)
			createStimuliWriter();

		String outLine = "0 1";
		out.write(outLine+"\n");
		out.flush();
		System.out.println("Written stimulus: "+outLine);
		
		if(cycles++ > 5)
			hasNextStimulus = false;
	}
	
	private void fetchResponse() throws IOException 
	{
		if(in == null)
			createResponseReader();
		
		String inLine = in.readLine();
		if(inLine == null)
			hasNextStimulus = false;
		System.out.println("Fetched response: "+inLine);
	}
	
	private void createStimuliWriter() throws FileNotFoundException 
	{
		out = new PrintWriter(new BufferedOutputStream(new FileOutputStream(stimuliFileName)));
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
