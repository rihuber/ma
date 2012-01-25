
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <mti.h>

int stimuliPipe = -1;
char* stimuliFileName;
char nextCharacter;
int nextErrorCode;

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

void initPipes(mtiVariableIdT arg_stimuliFileName, mtiVariableIdT arg_responseFileName)
{
	stimuliFileName = get_string(arg_stimuliFileName);
	nextErrorCode = 1;
}

int checkEndOfStimuliFile()
{
	return nextErrorCode;
}

void openStimuliPipe()
{
	stimuliPipe = open(stimuliFileName, O_RDONLY);
	if(stimuliPipe < 0)
		return;

	int flags = fcntl(stimuliPipe, F_GETFL);
	flags &= ~O_NONBLOCK;
	fcntl(stimuliPipe, F_SETFL, flags);

	nextErrorCode = readOneCharacter(&nextCharacter);
}


void readNextStimulusString(mtiVariableIdT stimulus, int* errorCode)
{
	if(stimuliPipe == -1)
		openStimuliPipe();
	if(stimuliPipe < 0)
	{
		printf("File descriptor of stimuli pipe broken\n");
		*errorCode = stimuliPipe;
		return;
	}

	int numCharacters = mti_TickLength(mti_GetVarType(stimulus));
	char* characters = mti_GetArrayVarValue(stimulus, NULL);

	int i = 0;
	while(i < numCharacters)
	{
		characters[i++] = nextCharacter;
		*errorCode = nextErrorCode;
		nextErrorCode = readOneCharacter(&nextCharacter);
	}
}

int readOneCharacter(char* result)
{
	int errorCode;
	do {
		errorCode = read(stimuliPipe, result, 1);
	} while((*result == ' ' || *result == '\n') && errorCode > 0);

	return errorCode;
}

void closeStimuliFile()
{
	close(stimuliPipe);
}
