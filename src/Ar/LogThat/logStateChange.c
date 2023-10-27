/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: LogThat
 * File: logStateChange.c
 * Author: ScismD, sClaiborne
 * Created: June 10, 2016
 ********************************************************************
 * Implementation of library LogThat
 ********************************************************************/

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

// First call - looks good
// Other calls - 

void logStateChange(struct logStateChange* inst) {
	
	StrExtArgs_typ LogData;
	
	LogData.i[0]= (DINT)inst->oldState;
	LogData.i[1]= (DINT)inst->State;
	
	LogData.s[0]= (UDINT)inst->ModuleName;
	LogData.s[1]= (UDINT)inst->oldStateName;
	LogData.s[2]= (UDINT)inst->StateName;
	
	// First call
	if (!inst->initialized) {
		
		// Default logger and module name
		// TODO: This seems like a bad default given the changes
		if (strcmp(inst->LoggerName, "") == 0) strcpy(inst->LoggerName, "State");	
		if (strcmp(inst->ModuleName, "") == 0) strcpy(inst->ModuleName, "State");
	
		// Initialize old data
		inst->oldState = inst->State;
		strcpy(inst->oldStateName, inst->StateName);
		
		if (strcmp(inst->StateName, "") == 0) {
			// TODO: msgData is not currently supported
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
