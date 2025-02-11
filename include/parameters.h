#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <json-c/json.h>
#include "list.h"


void init_parameters(void);
void init_proxy(struct sockaddr_in *);
void init_upstream(struct sockaddr_in *);
void init_blacklist(struct list *);
void init_response(uint8_t *);
const char *read_file(const char *);
void solve(char *, struct sockaddr_in *);


#endif