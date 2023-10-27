/*
 * File: LogInternalWrite.c
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
	#include <string.h>
	#include "LogInternalWrite.h"
#ifdef __cplusplus
	};
#endif

#define CUSTOMER_BIT_OFFSET 29

// Writes to log from severity, code, and template string
signed long LogInternalWrite(plcstring* loggerName, unsigned char severity, unsigned short facility, unsigned short code, plcstring* message, unsigned long pMsgData) {
	return LogInternalWriteMsgDataSwitch(loggerName, ArEventLogMakeEventID(severity, facility, code), message, pMsgData);
}

// Writes to log from event ID and optionally a template string
signed long LogInternalWriteMsgDataSwitch(plcstring* loggerName, signed long eventID, plcstring* message, unsigned long pMsgData) {
	if(pMsgData) {
		// Call a function here instead of formatting the message to... 
		//  avoid allocating temp string to stack without need
		return LogInternalWriteMsgData(loggerName, eventID, message, pMsgData);
		
	} else {
		// If we dont have pMsgData then we can not format message
		return 	LogInternalWriteEvent(loggerName, eventID, message);
	}
}

// Formats message with message data then writes to logger
signed long LogInternalWriteMsgData(plcstring* loggerName, signed long eventID, plcstring* message, unsigned long pMsgData) {
	// Format message then call write to logger
	char formattedMsg[LOG_STRLEN_MESSAGE];
	formatString(formattedMsg, sizeof(formattedMsg), message, (StrExtArgs_typ*)pMsgData);
		
	return LogInternalWriteEvent(loggerName, eventID, (unsigned long)formattedMsg);
}

// Writes to log from event ID and message
signed long LogInternalWriteEvent(plcstring* loggerName, signed long eventID, plcstring* message) {
	ArEventLogWrite_typ WriteLog = {0};
	ArEventLogGetIdent_typ getIdent = {0};
	
	// Check invalid inputs
	if(!loggerName) return LOG_ERR_INVALIDINPUT;
	
	// Get Ident
	getIdent.Execute = 1;
	strcpy(getIdent.Name, loggerName);
	ArEventLogGetIdent(&getIdent); // FUB should be synchronous
	if(getIdent.StatusID != 0) return getIdent.StatusID;
	
	// Write event to logger
	WriteLog.Execute = 1;
	// From Help: Only customer IDs can be entered. System IDs are rejected with the error arEVENTLOG_ERR_EVENT_ID
	WriteLog.EventID = eventID | (1 << CUSTOMER_BIT_OFFSET); // Set customer bit while writing it to FUB
	WriteLog.Ident = getIdent.Ident;
	
	if(message) {
		WriteLog.AddDataSize = brsstrlen(message)+1;
		WriteLog.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
		WriteLog.AddData = message;
	}
	
	ArEventLogWrite(&WriteLog);
	
	return WriteLog.StatusID;
}
