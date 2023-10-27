
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

signed long logSuccess(plcstring* LoggerName, unsigned short ErrorID, plcstring* ErrorString, unsigned long pMsgData) {
	return LogInternalWrite(LoggerName, LOG_SEVERITY_SUCCESS, LOG_DEFAULT_FACILITY, ErrorID, ErrorString, pMsgData);
}
