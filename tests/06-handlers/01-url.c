/*
	Onion HTTP server library
	Copyright (C) 2010-2011 David Moreno Montero

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/

#include <string.h>

#include <onion/onion.h>
#include <onion/server.h>
#include <onion/handlers/url.h>
#include <onion/log.h>

#include "../test.h"




typedef struct{
	char *data;
	size_t size;
	off_t pos;
}buffer;

/// Just appends to the handler. Must be big enought or segfault.. Just for tests.
int buffer_append(buffer *handler, const char *data, unsigned int length){
	int l=length;
	if (handler->pos+length>handler->size){
		l=handler->size-handler->pos;
	}
	memcpy(handler->data+handler->pos,data,l);
	handler->pos+=l;
	return l;
}

buffer *buffer_new(size_t size){
	buffer *b=malloc(sizeof(buffer));
	b->data=malloc(size);
	b->pos=0;
	b->size=size;
	return b;
}

void buffer_free(buffer *b){
	free(b->data);
	free(b);
}

buffer *server_buffer;

int handler_called=0;
char *urltxt;

int handler1(void *p, onion_request *r){
	ONION_DEBUG("1");
	handler_called=1;
	urltxt=strdup(onion_request_get_path(r));
	onion_response *res=onion_response_new(r);
	return onion_response_free(res);
}

int handler2(void *p, onion_request *r){
	ONION_DEBUG("2");
	handler_called=2;
	urltxt=strdup(onion_request_get_path(r));
	onion_response *res=onion_response_new(r);
	return onion_response_free(res);
}

int handler3(void *p, onion_request *r){
	ONION_DEBUG("3");
	handler_called=3;
	urltxt=strdup(onion_request_get_path(r));
	onion_response *res=onion_response_new(r);
	return onion_response_free(res);
}

onion_server *server;

void t01_url(){
	INIT_LOCAL();
	
	onion_handler *url=onion_handler_url();
	onion_handler_url_add(url, "^/handler1/$", onion_handler_new((onion_handler_handler)handler1, NULL, NULL));
	onion_handler_url_add(url, "^/handler2/$", onion_handler_new((onion_handler_handler)handler2, NULL, NULL));
	onion_handler_url_add(url, "^/handler3/", onion_handler_new((onion_handler_handler)handler3, NULL, NULL));
	
	onion_server_set_root_handler(server, url);
	
	onion_request *req=onion_request_new(server, server_buffer, "test");
	
#define R "GET /handler1/ HTTP/1.1\n\n"
	onion_request_write(req,R,sizeof(R));
	FAIL_IF_NOT_EQUAL_INT(handler_called, 1);
	FAIL_IF_NOT_EQUAL_STR(urltxt, "");
	free(urltxt);
	
	onion_request_clean(req);
	onion_request_write(req,"GET /handler2/ HTTP/1.1\n\n",sizeof(R));
	FAIL_IF_NOT_EQUAL_INT(handler_called, 2);
	ONION_DEBUG("%s", urltxt);
	FAIL_IF_NOT_EQUAL_STR(urltxt, "");
	free(urltxt);
	
	onion_request_clean(req);
	onion_request_write(req,"GET /handler3/hello HTTP/1.1\n\n",sizeof(R)+5);
	FAIL_IF_NOT_EQUAL_INT(handler_called, 3);
	FAIL_IF_NOT_EQUAL_STR(urltxt, "hello");
	free(urltxt);
	
	onion_request_free(req);
	onion_handler_free(url);
	onion_server_set_root_handler(server, NULL);
	
	END_LOCAL();
}

void init(){
	server=onion_server_new();
	server_buffer=buffer_new(4096);
	onion_server_set_write(server, (void*)&buffer_append);
}

void end(){
	buffer_free(server_buffer);
	onion_server_free(server);
}

int main(int argc, char **argv){
	init();
	t01_url();
	
	end();
	END();
}