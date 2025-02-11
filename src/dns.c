#include "dns.h"


struct dns_header get_header(void *buffer){
	struct dns_header h = *((struct dns_header *) buffer);

	h.id = ntohs(h.id);
	h.flags = ntohs(h.flags);
	h.qdcount = ntohs(h.qdcount);
	h.ancount = ntohs(h.ancount);
	h.nscount = ntohs(h.nscount);
	h.arcount = ntohs(h.arcount);

	return h;
}


struct question_record get_question_record(uint8_t *buffer){
	uint8_t *p = buffer;
	int size = 1, words = 0;
	while(*p){
		uint8_t cnt = *p;

		words++;
		size += cnt + 1;
		p += cnt + 1;
	}

	// printf("size: %d\n", size);

	p = buffer;
	char *name = malloc(size - 1);
	name[size - 2] = '\0';
	for(int i=0; *p;){
		uint8_t cnt = *p;
		for(int j=1; j<=cnt; ++j) name[i++] = p[j];
		if(i < size-2) name[i++] = '.';

		p += cnt + 1;
	}


	struct question_record r;
	r.name = name;
	r.type = ntohs(*((uint16_t *) (buffer+size)));
	r.class = ntohs(*((uint16_t *) (buffer+size+2)));


	return r;
}


void print_dns_header(struct dns_header *h){
	printf("header:\n");
	printf("id: 0x%04X\n", h->id);
	printf("flags: 0x%04X\n", h->flags);
	printf("\tqr: %d\n", h->qr);
	printf("\topcode: %d\n", h->opcode);
	printf("\tAA: %d\n", h->aa);
	printf("\tTC: %d\n", h->tc);
	printf("\tRD: %d\n", h->rd);
	printf("\tRA: %d\n", h->ra);
	printf("\tZ: %d\n", h->z);
	printf("\tAD: %d\n", h->ad);
	printf("\tCD: %d\n", h->cd);
	printf("\trcode: %d\n", h->rcode);
	printf("qdcount: %X\n", h->qdcount);
	printf("ancount: %X\n", h->ancount);
	printf("nscount: %X\n", h->nscount);
	printf("arcount: %X\n", h->arcount);
}


void print_question_record(struct question_record *r){
	printf("question record:\n");
	printf("\tdomain: %s\n", r->name);
	printf("\ttype: %d\n", r->type);
	printf("\tclass: %d\n", r->class);
}


const char *response_code_to_string(uint8_t x){
	int length = 20;
	char *s = malloc(length);
	memset(s, 0, length);


	switch(x){
		case 1:
			s = "Format error";
			break;

		case 2:
			s = "Server failure";
			break;

		case 3:
			s = "Name Error";
			break;

		case 4:
			s = "Not Implemented";
			break;

		case 5:
			s = "Refused";
			break;

		default:
			fprintf(stderr, "wrong value of response parameter\n");
			exit(EXIT_FAILURE);
	}


	return s;
}