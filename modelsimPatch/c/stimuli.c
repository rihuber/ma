
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <mti.h>

int pipeHandle = -1;
char* fileName;
int endOfFile = 0;

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

void initStimuli(mtiVariableIdT arg_stimuliFileName)
{
	fileName = get_string(arg_stimuliFileName);
}

int checkEndOfFile()
{
	return endOfFile;
}

void openStimuli()
{
	pipeHandle = open(fileName, O_RDONLY);
	if(pipeHandle < 0)
		return;

	int flags = fcntl(pipeHandle, F_GETFL);
	flags &= ~O_NONBLOCK;
	fcntl(pipeHandle, F_SETFL, flags);
}


void readNextTestString(mtiVariableIdT stimulus, int* errorCode)
{

	if(pipeHandle == -1)
		openStimuli();
	if(pipeHandle < 0)
	{
		printf("ERROR: File descriptor of stimuli pipe broken\n");
		*errorCode = pipeHandle;
		return;
	}

	int numCharacters = mti_TickLength(mti_GetVarType(stimulus));
	char* characters = mti_GetArrayVarValue(stimulus, NULL);

	int i = 0;
	while(i < numCharacters)
	{
		*errorCode = readOneCharacter(&(characters[i++]));
	}
	if(*errorCode == 0)
		endOfFile = 1;
}

int readOneCharacter(char* result)
{
	int errorCode;
	do {
		errorCode = read(pipeHandle, result, 1);
	} while((*result == 32 || *result == 10) && errorCode > 0);

	return errorCode;
}

void closeStimuli()
{
	close(pipeHandle);
	pipeHandle = -1;
}
