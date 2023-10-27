/*
 * File: logManagement.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of LogThat, licensed under the MIT License.
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

signed long createLogInit(plcstring* name, unsigned long size, enum LOG_PERSISTENCE_enum persistence) { 
	// Check inputs and set defaults 
	if(!name) return LOG_ERR_INVALIDINPUT;
	if(!size) size = LOG_DEFAULT_LOGGERSIZE;
	
	// Max strlen of name can not be greater than 10 chars
	// We should maybe handle this here
	ArEventLogCreate_typ createLog = {};
	createLog.Execute = 1;
	strcpy(createLog.Name, (char*)name);
	createLog.Info = 0;
	createLog.Persistence = persistence;
	createLog.Size = size;
	ArEventLogCreate(&createLog);
	return createLog.StatusID;
}

void logDelete(logDelete_typ* inst) {
	
	if(inst->execute) {
		
		strcpy(inst->ident.Name, inst->name);
		
		// Handle ident here
		// This should be a synchronous call
		inst->ident.Execute = 1;
		ArEventLogGetIdent(&inst->ident);
		
		// Handle done, Error is handled below
		if(inst->ident.Done) {
			inst->deletion.Execute = 1;
			inst->deletion.Ident = inst->ident.Ident;
		}
		
	}
	else {
		// Reset Ident
		inst->ident.Execute = 0;
		ArEventLogGetIdent(&inst->ident);
		
		
		// Reset delete
		inst->deletion.Execute = 0;
		
		
		// Reset errors
		inst->error = 0;
		inst->errorID = 0;
	}
	
	// Call FUBs
	ArEventLogDelete(&inst->deletion);
	
	// Set outputs
	if(inst->ident.Error) {
		inst->error = 1;
		inst->errorID = inst->ident.StatusID;
	}
	else if(inst->deletion.Error) {
		inst->error = 1;
		inst->errorID = inst->deletion.StatusID;
	}
	
	inst->busy = inst->deletion.Busy;
	inst->done = inst->deletion.Done;
}
