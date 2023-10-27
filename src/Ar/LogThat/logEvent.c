
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
