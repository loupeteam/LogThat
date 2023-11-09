/*
 * File: logEvent.c
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
#ifdef __cplusplus
	};
#endif

signed long logEventID(plcstring* loggerName, signed long eventID, plcstring* eventString, unsigned long pMsgData) {
	return LogInternalWriteMsgDataSwitch(loggerName, eventID, eventString, pMsgData);
}
