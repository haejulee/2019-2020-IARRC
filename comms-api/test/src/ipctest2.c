#include <comms.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PUB_PATH "../test/config/pub_ipc.json"
#define SUB_PATH "../test/config/sub_ipc.json"

void print_configs(topic_info_array_t *configs);
void print_message(msg_t *msg);
void recv(char *topic, int flag, char *s_flag, void *node);
void send(char *topic, void *msg, int size, void *node);


void subscriber() {
        init_comms();
        void *snode = init_node(SUB_PATH, NULL);
        if (snode == NULL) {
                perror("subscriber: error in init_node:");
                exit(1);
        }
        printf("subscriber socket initialized\n");
	while (1) {
        recv("sub1", 0, "blocking", snode);
	}
}

int main() {
	subscriber();
	return 0;
}

void print_configs(topic_info_array_t *configs) {
        printf("number of topics: %d\n", configs->num_topics);
        for(int i=0; i<configs->num_topics; i++) {
                printf("%s\n\trole: %s\n\ttransport: %s\n\taddress: %s\n",
                        configs->topics[i]->name,
                        configs->topics[i]->role,
                        configs->topics[i]->transport,
                        configs->topics[i]->address);
        }
}

void print_message(msg_t *msg) {
        assert(msg != NULL);
        assert(msg->msg != NULL);
        printf("printing message:\n");
        printf("%s", (char *)(msg->msg));
}

void recv(char *topic, int flag, char *s_flag, void *node) {
        printf("receiving message on %s...\n", topic);
        msg_t *msg = subscribe(topic, flag, node);
        if (msg == NULL) perror(s_flag);
        else {
                printf("message received!\n");
                print_message(msg);
                close_msg(msg);
        }
}

void send(char *topic, void *msg, int size, void *node) {
        if (publish(topic, msg, size, 0, node) != 0) {
                perror("publish:");
                return;
        }
        printf("message sent on %s\n", topic);
}

