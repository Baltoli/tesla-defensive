#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include "lib.h"

/**
 * This is the plugin's only interface to the server - from here it can call any
 * methods exposed by the server's API.
 */
void handle_message(char *message);

#endif
