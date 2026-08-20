/*
 * File: logStateChange.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of LogThat, licensed under the MIT License.
 * 
 */

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "LogThat.h"
#include "LogInternalWrite.h"
#include <string.h>

#ifdef __cplusplus
	};
#endif

//------------------------------------------
// Monitor a state variable and log changes
//------------------------------------------

void logStateChange(struct logStateChange* inst) {
	
	StrExtArgs_typ LogData;
	unsigned char firstCall = !inst->initialized;
	
	// Seed defaults and stored state before building LogData, so the start
	//  message reports the state present on the first call
	if (firstCall) {
		
		// Default logger and module name
		if (strcmp(inst->LoggerName, "") == 0) strcpy(inst->LoggerName, "State");	
		if (strcmp(inst->ModuleName, "") == 0) strcpy(inst->ModuleName, "State");
		
		// Initialize old data
		inst->oldState = inst->State;
		strcpy(inst->oldStateName, inst->StateName);
		
	}
	
	memset(&LogData, 0, sizeof(LogData));
	
	LogData.i[0]= (DINT)inst->oldState;
	LogData.i[1]= (DINT)inst->State;
	
	LogData.s[0]= (UDINT)inst->ModuleName;
	LogData.s[1]= (UDINT)inst->oldStateName;
	LogData.s[2]= (UDINT)inst->StateName;
	
	// First call
	if (firstCall) {
		
		if (strcmp(inst->StateName, "") == 0) {
			inst->Status = logInfo(inst->LoggerName, 0, "%s start in state %i", (UDINT)&LogData);
		} else {
			inst->Status = logInfo(inst->LoggerName, 0, "%s start in state %s (%i)", (UDINT)&LogData);
		}
		
		inst->initialized = 1;
		
	}
	
	// Catch change of state
	if (inst->oldState != inst->State) {
		
		// Log state change
		if (strcmp(inst->StateName, "") == 0) {
			inst->Status = logInfo(inst->LoggerName, 0, "%s change from %i to %i", (UDINT)&LogData);
		} else {
			inst->Status = logInfo(inst->LoggerName, 0, "%s change from %s to %s (%i to %i)", (UDINT)&LogData);
		}
		
		inst->oldState = inst->State;
		strcpy(inst->oldStateName, inst->StateName);

	}
	
} // End Fn
