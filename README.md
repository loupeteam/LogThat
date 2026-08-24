# Info
Library is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
LogThat provides an easy to use interface for adding diagnostic information into the CPU's logger. It wraps B&R's ArEventLog library so creating a logbook takes one line in `_INIT` and writing an entry takes a single function call. Messages can include runtime values through a printf-style format string, using `StrExtArgs_typ` from Loupe's StringExt library.

For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/logthat.html

# Quick start

Create the logbook once from an `_INIT` routine. Names are limited to 8 characters, and the log data area is 4096 bytes minimum (passing `0` uses `LOG_DEFAULT_LOGGERSIZE`, 100000). Use `LOG_PERSISTENCE_PERSIST` unless you have a reason not to: log entries matter most after the restart that followed the problem, and a volatile logbook is empty by then.

```c
void _INIT ProgramInit(void)
{
	createLogInit("App", 100000, LOG_PERSISTENCE_PERSIST);
}
```

Note that persistent and remanent entries live in a DRAM image copied to the backing memory every 60 seconds and on an orderly shutdown, so up to a minute of entries can still be lost to a power fail or watchdog reset.

Write entries from anywhere. The second argument is a user defined code written into the event ID, and the last argument is a pointer to format arguments (`0` when the message has no runtime values).

```c
void _CYCLIC ProgramCyclic(void)
{
	logInfo("App", 0, "Machine started", 0);
	logWarning("App", 100, "Infeed sensor blocked", 0);
	logError("App", 200, "Drive fault, machine stopped", 0);
}
```

To include runtime values, fill in a `StrExtArgs_typ` and pass its address. Each specifier consumes the next unused member of the matching type: `%i`/`%d` from `i[]`, `%r`/`%f` from `r[]`, `%s` from `s[]` (string addresses), and `%b` from `b[]`. There are five members of each type, and a sixth specifier of the same type is dropped from the message without an error.

Formatting happens only when `pMsgData` is non-zero. With `0` the message is written to the logger verbatim, percent signs included. When you do pass arguments, use `%%` for a literal percent: an unrecognized `%` is dropped together with the single character that follows it, so a stray `%` in operator text eats the next character.

```c
void _CYCLIC ProgramCyclic(void)
{
	unsigned short errorCount = 3;
	plcstring recipeName[32] = "Widget";

	StrExtArgs_typ msgData;
	memset(&msgData, 0, sizeof(msgData));

	msgData.i[0] = errorCount;
	msgData.s[0] = (UDINT)recipeName;

	logWarning("App", 300, "Recipe %s reported %i faults", (UDINT)&msgData);
	// -> "Recipe Widget reported 3 faults"
}
```

The log functions return a status. `0` means the entry was written; `LOG_ERR_INVALIDINPUT` (58300) means a required input was missing, and any other value is passed through from ArEventLog (most often because the logbook has not been created).

# Naming a logbook

Logbooks are stored as Automation Runtime modules and share one namespace with every other module on the target, including the tasks and programs in the project. Creating a logbook named after an existing task fails with `arEVENTLOG_ERR_MODULE_EXISTS` (-1070586084) and no logbook is created, so every later write fails too. Pick a name that is not a task, program, or module name, keep it within 8 characters, and do not start it with `$`.

A logbook that outlives the restart which reran `_INIT` is still there the next time `createLogInit` runs, so the create reports `arEVENTLOG_ERR_LOGBOOK_EXISTS` (-1070586095). `LOG_PERSISTENCE_PERSIST` survives a cold restart and `LOG_PERSISTENCE_REMANENT` survives a warm one, so with either of those this is the normal case rather than an error:

```c
void _INIT ProgramInit(void)
{
	DINT status = createLogInit("App", 100000, LOG_PERSISTENCE_PERSIST);

	if (status != 0 && status != arEVENTLOG_ERR_LOGBOOK_EXISTS) {
		// Create failed for a real reason, most likely a name collision
	}
}
```

`createLogInit` calls `ArEventLogCreate` once and returns that call's status rather than polling the function block to completion, and B&R documents `ArEventLogCreate` as asynchronous. Treat an unexpected status here as a prompt to check the status of the first write, which is where a missing logbook shows up unambiguously.

# Dependencies
- ArEventLog and AsBrStr, both shipped with Automation Studio
- Loupe's [StringExt](https://github.com/loupeteam/StringExt) library, used for message formatting

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install logthat`. LogThat's package does not declare StringExt as an LPM dependency, so install it as well with `lpm install stringext` if the project does not already have it. For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html

## Licensing

This project is licensed under the [MIT License](LICENSE).
