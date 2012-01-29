package ch.rihuber.vhdl;


public class StdLogicVector implements VhdlDataType
{
	
	private StdLogic[] value;
	
	public StdLogicVector(String stringValue)
	{
		parseString(stringValue);
	}
	
	public StdLogicVector(StdLogic[] value)
	{
		this.value = value;
	}
	
	public StdLogicVector(int value, int size) throws Exception 
	{
		int valueCopy = value;
		String stringValue = Integer.toBinaryString(valueCopy);

		if(stringValue.length() > size)
			throw new Exception("Unable to represent integer value of " + value + " with std_logic_vector with " + size + " elements!");
		
		while(stringValue.length() < size)
		{
			stringValue = "0" + stringValue;
		}
		
		parseString(stringValue);
	}

	public StdLogicVector(StdLogic defaultValue, int size) 
	{
		value = new StdLogic[size];
		for(int i=0; i<size; i++)
			value[i] = defaultValue;
	}

	public StdLogic valueAt(int index)
	{
		return value[index];
	}

	public StdLogicVector subVector(int left, int right) throws Exception 
	{
		if(left < right)
			throw new Exception("Unable to extract subVector: left("+left+") is smaller than right("+right+")!");
		
		StdLogic[] result = new StdLogic[left-right+1];
		for(int i=right; i<=left; i++)
			result[i-right] = value[i];
		
		return new StdLogicVector(result);
	}

	public int toInteger() throws Exception 
	{
		if(value.length > 31)
			throw new Exception("Unable to parse std_logic_vector to integer, std_logic_vector is too large ("+value.length+")");
		
		int result = 0;
		for(int i=value.length-1; i>=0; i--)
		{
			result = (result << 1) | value[i].toInteger();
		}
		
		return result;
	}
	
	private void parseString(String stringValue)
	{
		value = new StdLogic[stringValue.length()]; 
		for(int i=0; i<stringValue.length(); i++)
		{
			int substringIndex = stringValue.length()-(i+1);
			String currentValue = stringValue.substring(substringIndex, substringIndex+1);
			value[i] = StdLogic.create(currentValue);
		}
	}

	public void setValueAt(int index, StdLogic value) 
	{
		this.value[index] = value;
	}
	
	public String toString()
	{
		String result = "";
		for(int i=0; i<value.length; i++)
			result = value[i].toString() + result;
		return result;
	}
}

