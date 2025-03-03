#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "dns.h"
#include "list.h"
#include "parameters.h"
#define BUFFER_SIZE 4096


static uint8_t response_code;
static struct list blacklist;
static struct sockaddr_in proxy_server_addr, upstream_server_addr;


int main(){
	memset(&proxy_server_addr, 0, sizeof proxy_server_addr);
	memset(&upstream_server_addr, 0, sizeof upstream_server_addr);
	memset(&blacklist, 0, sizeof blacklist);


	init_parameters();
	init_proxy(&proxy_server_addr);
	init_upstream(&upstream_server_addr);
	init_blacklist(&blacklist);
	init_response(&response_code);


	printf("Proxy Server Address:\n");
	printf("\tip: %s\n", inet_ntoa(proxy_server_addr.sin_addr));
	printf("\tport: %hu\n", ntohs(proxy_server_addr.sin_port));
	putchar('\n');


	printf("Upstream Server Address:\n");
	printf("\tip: %s\n", inet_ntoa(upstream_server_addr.sin_addr));
	printf("\tport: %hu\n", ntohs(upstream_server_addr.sin_port));
	putchar('\n');


	printf("Blacklist:\n");
	for(int i=0; i<blacklist.count; ++i) printf("\t- %s\n", blacklist.names[i]);
	putchar('\n');


	printf("Response code: %hhu\n", response_code);
	printf("\tinformation: %s\n", response_code_to_string(response_code));
	putchar('\n');


	int proxy_server_fd_for_clients = socket(AF_INET, SOCK_DGRAM, 0);
	if(proxy_server_fd_for_clients < 0){
		perror("socket for clients");
		exit(EXIT_FAILURE);
	}


	printf("UDP socket for clients created\n");


	int proxy_server_fd_for_upstream = socket(AF_INET, SOCK_DGRAM, 0);
	if(proxy_server_fd_for_upstream < 0){
		perror("socket for upstream");
		close(proxy_server_fd_for_clients);
		exit(EXIT_FAILURE);
	}


	printf("UDP socket for upstream created\n");


	if(bind(proxy_server_fd_for_clients, (struct sockaddr *) &proxy_server_addr, sizeof(proxy_server_addr)) < 0){
		perror("bind for clients");
		close(proxy_server_fd_for_clients);
		close(proxy_server_fd_for_upstream);
		exit(EXIT_FAILURE);
	}


	printf("UDP socket for clients binded\n");


	for(;;){
		ssize_t n;
		uint8_t buffer[BUFFER_SIZE];
		socklen_t src_addrlen;
		struct sockaddr_in src_addr;
		src_addrlen = sizeof src_addr;


		memset(buffer, 0, BUFFER_SIZE);
		memset(&src_addr, 0, sizeof src_addr);


		n = recvfrom(proxy_server_fd_for_clients, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &src_addr, &src_addrlen);
		if(n == -1){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		// printf("size of received data from client(%s): %lu\n", inet_ntoa(src_addr.sin_addr), n);


		const socklen_t client_addrlen = src_addrlen;
		const struct sockaddr_in client_addr = src_addr;


		struct dns_header header = get_header(buffer);
		struct question_record record = get_question_record(buffer + 12);
		if(header.qdcount != 1){
			printf("skip packet because of wrong QDCOUNT\n");

			continue;
		}


		printf("received query from %s:%u about %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), record.name);


		if(find_string_in_list(&blacklist, record.name)){
			struct dns_header *h = (struct dns_header *) buffer;

			h->flags = ntohs(h->flags);
			h->qr = true;
			h->rcode = response_code;
			h->flags = htons(h->flags);
		}
		else{
			n = sendto(proxy_server_fd_for_upstream, buffer, n, 0, (struct sockaddr *) &upstream_server_addr, sizeof upstream_server_addr);
			if(n == -1){
				perror("sendto");
				exit(EXIT_FAILURE);
			}
			// printf("size of sended data to upstream: %lu\n", n);


			n = recvfrom(proxy_server_fd_for_upstream, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &src_addr, &src_addrlen);
			if(n == -1){
				perror("recvfrom");
				exit(EXIT_FAILURE);
			}
			// printf("size of received data from upstream: %lu\n", n);
		}


		n = sendto(proxy_server_fd_for_clients, buffer, n, 0, (struct sockaddr *) &client_addr, client_addrlen);
		if(n == -1){
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		// printf("size of sended data to client(%s): %lu\n", inet_ntoa(client_addr.sin_addr), n);
	}


	close(proxy_server_fd_for_clients);
	close(proxy_server_fd_for_upstream);


	return 0;
}