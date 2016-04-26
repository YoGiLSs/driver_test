#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EQ_LOG_DBG

static int excuteCmd(char *cmd)
{
	#ifndef EQ_LOG_DBG
	strcat(cmd, " > /dev/null 2>&1");
	#endif
	return system(cmd);
}

static int updateUserBandGain(int bandIdx, int gainValue)
{
	char cmd[128];
	int ret;

	sprintf(cmd, "amxier sset 'User PEQ Band%d FC' %d", bandIdx, gainValue);
	ret = excuteCmd(cmd);

	return ret;
}

static int setEqBass(char params)
{
	return updateUserBandGain(3, params);
}


static int setEqMiddle(char params)
{
	return updateUserBandGain(5, params);
}

static int setEqTreble(char params)
{
	return updateUserBandGain(7, params);
}

int set_audio_params(AudioParamType type, AudioParams params)
{
	int ret;
	switch(type){
	case APT_EQ:
		ret = setEqBass(params.EQ_Params.eq_bass);
		if(ret)
			return 1;

		ret = setEqMiddle(params.EQ_Params.eq_middle);
		if(ret)
			return 1;

		ret = setEqTreble(params.EQ_Params.eq_treble);
		if(ret)
			return 1;
		break;
	default:
		printf("audio not support type %d \n", type);
		ret = 1;	
	}
	return ret;
}

