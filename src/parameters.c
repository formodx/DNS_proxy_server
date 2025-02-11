#include "parameters.h"


static struct json_object *json;


void init_parameters(void){
	const char *json_string = read_file("config.json");
	json = json_tokener_parse(json_string);
}


void init_proxy(struct sockaddr_in *addr){
	struct json_object *proxy_object;
	json_object_object_get_ex(json, "proxy", &proxy_object);

	const char *proxy_string = json_object_get_string(proxy_object);
	size_t proxy_string_length = strlen(proxy_string);

	// printf("proxy string: %s\n", proxy_string);
	// printf("proxy string length: %lu\n", proxy_string_length);

	char *proxy = malloc(proxy_string_length + 1);
	strcpy(proxy, proxy_string);
	solve(proxy, addr);
	free(proxy);
}


void init_upstream(struct sockaddr_in *addr){
	struct json_object *upstream_object;
	json_object_object_get_ex(json, "upstream", &upstream_object);

	const char *upstream_string = json_object_get_string(upstream_object);
	size_t upstream_string_length = strlen(upstream_string);

	// printf("upstream string: %s\n", upstream_string);
	// printf("upstream string length: %lu\n", upstream_string_length);

	char *upstream = malloc(upstream_string_length + 1);
	strcpy(upstream, upstream_string);
	solve(upstream, addr);
	free(upstream);
}


void init_blacklist(struct list *blacklist){
	struct json_object *blacklist_object;
	json_object_object_get_ex(json, "blacklist", &blacklist_object);

	size_t cnt = json_object_array_length(blacklist_object);

	create_list(blacklist, cnt);

	for(int i=0; i<cnt; ++i){
		struct json_object *e = json_object_array_get_idx(blacklist_object, i);

		const char *string = json_object_get_string(e);
		int length = json_object_get_string_len(e);

		char *s = malloc(length + 1);
		strcpy(s, string);

		blacklist->names[i] = s;
	}
}


void init_response(uint8_t *response){
	struct json_object *response_object;
	json_object_object_get_ex(json, "response", &response_object);

	*response = json_object_get_int(response_object);
}


const char *read_file(const char *pathname){
	FILE *file = fopen(pathname, "r");
	if(file == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0L, SEEK_END);
	int file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	printf("Config File Information:\n");
	printf("\tsize: %d bytes\n", file_size);
	putchar('\n');

	char *text = malloc(file_size + 1);
	if(text == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	fread(text, 1, file_size, file);
	text[file_size] = '\0';

	fclose(file);

	return text;
}


void solve(char *ip_port, struct sockaddr_in *addr){
	char *ip = ip_port, *port;
	for(;; ++ip_port){
		if(*ip_port == ':'){
			*ip_port = '\0';
			port = ip_port + 1;

			break;
		}
	}


	memset(addr, 0, sizeof *addr);
	addr->sin_family = AF_INET;
	if(inet_pton(AF_INET, ip, &(addr->sin_addr)) < 0){
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	addr->sin_port = htons(atoi(port));
}
