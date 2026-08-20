# Info
Library is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
LogThat provides an easy to use interface for adding diagnostic information into the CPU's logger. It wraps B&R's ArEventLog library so creating a logbook takes one line in `_INIT` and writing an entry takes a single function call. Messages can include runtime values through a printf-style format string, using `StrExtArgs_typ` from Loupe's StringExt library.

For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/logthat.html

# Quick start

Create the logbook once from an `_INIT` routine. Names are limited to 8 characters and the log data area is 4096 bytes minimum.

```c
void _INIT ProgramInit(void)
{
	createLogInit("App", 1000000, LOG_PERSISTENCE_VOLATILE);
}
```

Write entries from anywhere. The second argument is a user defined code written into the event ID, and the last argument is a pointer to format arguments (`0` when the message has no runtime values).

```c
logInfo("App", 0, "Machine started", 0);
logWarning("App", 100, "Infeed sensor blocked", 0);
logError("App", 200, "Drive fault, machine stopped", 0);
```

To include runtime values, fill in a `StrExtArgs_typ` and pass its address. Each specifier consumes the next unused member of the matching type: `%i`/`%d` from `i[]`, `%r`/`%f` from `r[]`, `%s` from `s[]` (string addresses), and `%b` from `b[]`.

```c
StrExtArgs_typ msgData;
memset(&msgData, 0, sizeof(msgData));

msgData.i[0] = errorCount;
msgData.s[0] = (UDINT)&recipeName;

logWarning("App", 300, "Recipe %s reported %i faults", (UDINT)&msgData);
```

Every function returns a status. `0` means the entry was written; `LOG_ERR_INVALIDINPUT` (58300) means a required input was missing, and any other value is passed through from ArEventLog (most often because the logbook has not been created).

# Dependencies
- ArEventLog and AsBrStr, both shipped with Automation Studio
- Loupe's [StringExt](https://github.com/loupeteam/StringExt) library, used for message formatting

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install logthat`. For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html

## Licensing

This project is licensed under the [MIT License](LICENSE).
