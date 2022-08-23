// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "demeter.h"

void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf)
{
    if (gathered_logs != NULL) {
		write_log_to_file(demeter_conf, "logs gathered", DEBUG, 3);
		write_log_to_file(demeter_conf, ((parsed_log_t *)gathered_logs->data)->unparsed_log, DEBUG, 3);
		write_log_to_file(demeter_conf, ((parsed_log_t *)gathered_logs->data)->log_time_str, DEBUG, 3);
	}
}