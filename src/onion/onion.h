/*
	Onion HTTP server library
	Copyright (C) 2010-2014 David Moreno Montero and othes

	This library is free software; you can redistribute it and/or
	modify it under the terms of, at your choice:
	
	a. the Apache License Version 2.0. 
	
	b. the GNU General Public License as published by the 
		Free Software Foundation; either version 2.0 of the License, 
		or (at your option) any later version.
	 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of both libraries, if not see 
	<http://www.gnu.org/licenses/> and 
	<http://www.apache.org/licenses/LICENSE-2.0>.
	*/

#ifndef ONION_ONION_H
#define ONION_ONION_H

#include "request.h"
#include "response.h"
#include "handler.h"
#include "url.h"

#include "types.h"

#ifdef __cplusplus
extern "C"{
#endif

/// Creates the onion structure to fill with the server data, and later do the onion_listen()
onion *onion_new(int flags);

/// Performs the listening with the given mode
int onion_listen(onion *server);

/// Stops the listening
void onion_listen_stop(onion *server);

/// Removes the allocated data
void onion_free(onion *onion);

/// Sets the root handler
void onion_set_root_handler(onion *server, onion_handler *handler);

/// Sets the root handler
onion_handler *onion_get_root_handler(onion *server);

/// Sets the root handler
void onion_set_internal_error_handler(onion *server, onion_handler *handler);

/// Sets the port to listen
void onion_set_port(onion *server, const char *port);

/// Sets the hostname on which to listen
void onion_set_hostname(onion *server, const char *hostname);

/// Set a certificate for use in the connection
int onion_set_certificate(onion *onion, onion_ssl_certificate_type type, const char *filename, ...);

/// Adds a listen point, a listening address and port with a given protocol.
int onion_add_listen_point(onion *server, const char *hostname, const char *port, onion_listen_point *protocol);

/// Gets a single listen point, or NULL if not that many.
onion_listen_point *onion_get_listen_point(onion *server, int nlisten_point);

/// Gets the current flags, for example to check SSL support.
int onion_flags(onion *onion);

/// Sets the timeout, in milliseconds, 0 dont wait for incomming data (too strict maybe), -1 forever, clients closes connection
void onion_set_timeout(onion *onion, int timeout);

/// Sets the maximum number of threads to use for requests. default 16.
void onion_set_max_threads(onion *onion, int max_threads);

/// Sets this user as soon as listen starts.
void onion_set_user(onion *server, const char *username);

/// If no root handler is set, creates an url handler and returns it.
onion_url *onion_root_url(onion *server);

/// If on poller mode, returns the poller, if not, returns NULL
onion_poller *onion_get_poller(onion *server);

/// Set the maximum post size
void onion_set_max_post_size(onion *server, size_t max_size);

#ifdef __cplusplus
}
#endif


#endif
