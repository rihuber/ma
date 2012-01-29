package ch.rihuber.vhdl;

public enum StdLogic implements VhdlDataType
{
	ONE,
	ZERO,
	DONT_CARE,
	UNDEFINED;
	
	private static final String ONE_STRING = "1";
	private static final String ZERO_STRING = "0";
	private static final String DONT_CARE_STRING = "-";
	private static final String UNDEFINED_STRING = "U";
	
	public static StdLogic create(String value)
	{
		if(value.equals(ONE_STRING))
			return ONE;
		if(value.equals(ZERO_STRING))
			return ZERO;
		if(value.equals(DONT_CARE_STRING))
			return DONT_CARE;
		return UNDEFINED;
	}
	
	public static VhdlDataType create(boolean value) 
	{
		return (value) ? ONE : ZERO;
	}
	
	public boolean toBoolean() throws Exception
	{
		switch(this)
		{
			case ONE:	return true;
			case ZERO:	return false;
			default: 	throw new Exception("Unable to parse std_logic to boolean. std_logic value is " + this.toString());
		}
	}
	
	public String toString()
	{
		switch(this)
		{
			case ONE: 		return ONE_STRING;
			case ZERO: 		return ZERO_STRING;
			case DONT_CARE: return DONT_CARE_STRING;
			default: 		return UNDEFINED_STRING;
		}
	}
	
	public int toInteger() throws Exception 
	{
		switch(this)
		{
			case ONE:	return 1;
			case ZERO:	return 0;
			default:	throw new Exception("Unable to parse std_logic to integer. std_logic value is " + this.toString());
		}
	}
}

