(*
 * File: LogThat.fun
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of LogThat, licensed under the MIT License.
 *)

FUNCTION_BLOCK logDelete (*Delete a logbook and all of its entries. Call cyclically, holding execute until done or error, then clear it. Outputs keep following the underlying ArEventLog FUBs after execute is cleared*)
	VAR_INPUT
		name : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook to be deleted*)
		execute : BOOL; (*Hold TRUE until done or error. Clearing it early stops driving the delete without cancelling it*)
	END_VAR
	VAR_OUTPUT
		done : BOOL; (*Delete finished successfully*)
		busy : BOOL; (*Delete in progress*)
		error : BOOL; (*An error has occurred*)
		errorID : DINT; (*Status of the failed operation, passed through from ArEventLog*)
	END_VAR
	VAR
		ident : ArEventLogGetIdent;
		deletion : ArEventLogDelete;
	END_VAR
END_FUNCTION_BLOCK

{REDUND_CONTEXT} FUNCTION createLogInit : DINT (*Only supported in INIT*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook to be created (max 8 chars)*)
		size : UDINT; (*Length of the log data area in bytes (min 4096). 0 uses LOG_DEFAULT_LOGGERSIZE*)
		persistence : LOG_PERSISTENCE_enum; (*Where the entries are stored*)
	END_VAR
END_FUNCTION

{REDUND_CONTEXT} FUNCTION logEventID : DINT (*Write an event message to the logger*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook the message is added to*)
		eventID : DINT; (*Event ID containing severity, facility, and code. The customer bit is set internally*)
		eventString : STRING[LOG_STRLEN_MESSAGE]; (*Event message, when pMsgData is supplied it may contain formatters (%i %d %r %f %s %b) and %% for a literal percent*)
		pMsgData : UDINT; (*Address of a StrExtArgs_typ with the format arguments, max 5 of each type (i/r/s/b). 0 writes the message verbatim, with no formatting*)
	END_VAR
END_FUNCTION

{REDUND_CONTEXT} FUNCTION logSuccess : DINT (*Write a success message to the logger*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook the message is added to*)
		errorID : UINT; (*User defined code written into the event ID*)
		errorString : STRING[LOG_STRLEN_MESSAGE]; (*Message for logger entry, when pMsgData is supplied it may contain formatters (%i %d %r %f %s %b) and %% for a literal percent*)
		pMsgData : UDINT; (*Address of a StrExtArgs_typ with the format arguments, max 5 of each type (i/r/s/b). 0 writes the message verbatim, with no formatting*)
	END_VAR
END_FUNCTION
(*Legacy*)

{REDUND_CONTEXT} FUNCTION logWarning : DINT (*Write a warning message to the logger*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook the message is added to*)
		errorID : UINT; (*User defined code written into the event ID*)
		errorString : STRING[LOG_STRLEN_MESSAGE]; (*Message for logger entry, when pMsgData is supplied it may contain formatters (%i %d %r %f %s %b) and %% for a literal percent*)
		pMsgData : UDINT; (*Address of a StrExtArgs_typ with the format arguments, max 5 of each type (i/r/s/b). 0 writes the message verbatim, with no formatting*)
	END_VAR
END_FUNCTION

{REDUND_CONTEXT} FUNCTION logError : DINT (*Write a fatal error message to the logger*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook the message is added to*)
		errorID : UINT; (*User defined code written into the event ID*)
		errorString : STRING[LOG_STRLEN_MESSAGE]; (*Message for logger entry, when pMsgData is supplied it may contain formatters (%i %d %r %f %s %b) and %% for a literal percent*)
		pMsgData : UDINT; (*Address of a StrExtArgs_typ with the format arguments, max 5 of each type (i/r/s/b). 0 writes the message verbatim, with no formatting*)
	END_VAR
END_FUNCTION

{REDUND_CONTEXT} FUNCTION logInfo : DINT (*Write an info message to the logger*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		loggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of the logbook the message is added to*)
		errorID : UINT; (*User defined code written into the event ID*)
		errorString : STRING[LOG_STRLEN_MESSAGE]; (*Message for logger entry, when pMsgData is supplied it may contain formatters (%i %d %r %f %s %b) and %% for a literal percent*)
		pMsgData : UDINT; (*Address of a StrExtArgs_typ with the format arguments, max 5 of each type (i/r/s/b). 0 writes the message verbatim, with no formatting*)
	END_VAR
END_FUNCTION

{REDUND_CONTEXT} FUNCTION_BLOCK logStateChange (*Log an info entry on the first call and whenever State changes after that. Call cyclically*) (*$GROUP=User*)
	VAR_INPUT
		LoggerName : STRING[LOG_STRLEN_LOGGERNAME]; (*Name of logger for messages to be added. If empty on the first call, 'State' is written back into this input, once only*)
		ModuleName : STRING[LOG_STRLEN_MODULENAME]; (*Name of module with state changes. If empty on the first call, 'State' is written back into this input, once only*)
		State : UDINT; (*Current state*)
		StateName : STRING[LOG_STRLEN_STATENAME]; (*State name or description (optional)*)
	END_VAR
	VAR
		oldState : UDINT;
		oldStateName : STRING[LOG_STRLEN_STATENAME];
		Status : UINT;
		initialized : BOOL;
	END_VAR
END_FUNCTION_BLOCK
