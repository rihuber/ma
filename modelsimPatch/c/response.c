
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <mti.h>

int pipeHandle = -1;
char* fileName;

/* Convert a VHDL String array into a NULL terminated string */
static char *get_string(mtiVariableIdT id)
{
    static char buf[1001];
    mtiTypeIdT type;
    int len;
    type = mti_GetVarType(id);
    len = mti_TickLength(type);
    if(len > 1000)
    {
    	printf("ERROR: VHDL string exceeds buffer length of %d, string not parsed!", 1000 );
    	buf[0] = 0;
    	return buf;
    }
	mti_GetArrayVarValue(id, buf);
	buf[len] = 0;
    return buf;
}

void initResponse(mtiVariableIdT arg_stimuliFileName, mtiVariableIdT arg_responseFileName)
{
	fileName = get_string(arg_stimuliFileName);
}

void openResponse()
{
	pipeHandle = open(fileName, O_WRONLY);
	if(pipeHandle < 0)
		return;

	int flags = fcntl(pipeHandle, F_GETFL);
	flags &= ~O_NONBLOCK;
	fcntl(pipeHandle, F_SETFL, flags);
}


void writeString(mtiVariableIdT response, int* errorCode)
{
	if(pipeHandle == -1)
		openResponse();
	if(pipeHandle < 0)
	{
		printf("ERROR: File descriptor of response pipe broken\n");
		*errorCode = pipeHandle;
		return;
	}

	int numCharacters = mti_TickLength(mti_GetVarType(response));
	write(pipeHandle, get_string(response), numCharacters+1);
}

void writeEndOfLine()
{
	if(pipeHandle == -1)
		openResponse();
	if(pipeHandle < 0)
	{
		return;
	}
	static char newLine[2];
	newLine[0] = '\n';
	newLine[1] = '\0';
	write(pipeHandle, newLine, 2);
}

void closeResponse()
{
	close(pipeHandle);
	pipeHandle = -1;
}
