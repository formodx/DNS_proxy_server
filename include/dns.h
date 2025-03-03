#ifndef DNS_H
#define DNS_H


#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct dns_header{
	uint16_t id;
	union{
		struct{
			uint16_t rcode: 4;
			uint16_t cd: 1;
			uint16_t ad: 1;
			uint16_t z: 1;
			uint16_t ra: 1;
			uint16_t rd: 1;
			uint16_t tc: 1;
			uint16_t aa: 1;
			uint16_t opcode: 4;
			uint16_t qr: 1;
		};
		uint16_t flags;
	};
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
};


struct question_record{
	char *name;
	uint16_t type;
	uint16_t class;
};


struct dns_header get_header(void *);
struct question_record get_question_record(uint8_t *);
void print_dns_header(struct dns_header *);
void print_question_record(struct question_record *);
const char *response_code_to_string(uint8_t);


#endif