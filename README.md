
# acct_gather_profile_demeter
Slurm plugin for harvesting data on your cluster's nodes like info about ram usage, oom state, parsed logs, etc...

## Conf:
A conf file can be provided to the plugin if placed at /etc/slurm/demeter.conf. 
This file has to be readable by the slurm user and has to be present on every slurmd as well as the slurm controller.

### This file can contain multiple parameters that you can define:

Verbose <------- syntax: *"Verbose=(uint < 99)"*. Sets the verbose level for the demeter logs.

LogStyle <------ syntax: *"LogStyle=FANCY/SIMPLE/SYSTEM"*. Sets the logging style for demeter logs.

LogFilePath <--- syntax: *"LogFilePath='path/to/an/existing/or/non/existing/file'"*. Sets a custom path for the log file that demeter outputs in.
