#define _GNU_SOURCE

#include "connmgr.h"


void *copy_connection(void *src_element) {
    pollData *copy = malloc(sizeof(pollData));
    copy->file_descriptors = ((pollData *) src_element)->file_descriptors;
    copy->last_record = ((pollData *) src_element)->last_record;
    copy->sensor_id = ((pollData *) src_element)->sensor_id;
    copy->socket_id = ((pollData *) src_element)->socket_id;
    return (void *) copy;
}

void free_connection(void **element) {
    free(*element);
}

int compare_connection(void *x, void *y) {
    return 0;
}

dplist_t *connections;

void connmgr_listen(int port_number) {
    FILE *fp = fopen("sensor_data_recv.txt", "w");

    connections = dpl_create(copy_connection, free_connection, compare_connection);

    tcpsock_t *server, *socket;
    pollData pollServer;
    pollReqDescr pollServerFD;
    bool active = true;

    if (tcp_passive_open(&server, port_number) != TCP_NO_ERROR) {
        printf("TCP server creation failed.\n");
    }

    if (tcp_get_sd(server, &(pollServerFD.fd)) != TCP_NO_ERROR) {
        printf("Socket not connected.\n");
    }

    pollServer.last_record = time(NULL);
    pollServer.socket_id = server;
    pollServer.file_descriptors = pollServerFD;
    pollServer.file_descriptors.events = POLLIN;

    dpl_insert_at_index(connections, (void *) &pollServer, 0, false);

    while (active) {
        int connectionCount = dpl_size(connections);

        for (int i = 0; i < connectionCount; i++) {
            pollData *currenElement = ((pollData *) dpl_get_element_at_index(connections, i));

            if (poll(&(currenElement->file_descriptors), 1, 0)) {
                if (currenElement->last_record + TIMEOUT < time(NULL)) {
                    tcp_close(&(currenElement->socket_id));
                    connections = dpl_remove_at_index(connections, i, true);
                    pollServer.last_record = time(NULL);
                    connectionCount = dpl_size(connections);
                    break;
                }

                if (currenElement->file_descriptors.revents == POLLIN) {
                    // Index 0 is the server. In this if block, I just check if there is incoming socket request. If that
                    // is the case, I am adding that new connection to the connections list.
                    if (i == 0) {
                        tcp_wait_for_connection(currenElement->socket_id, &socket);
                        pollData tempSocket;
                        tempSocket.socket_id = socket;
                        if (tcp_get_sd(socket, &(pollServerFD.fd)) != TCP_NO_ERROR) {
                            printf("Socket not connected.\n");
                        }

                        tempSocket.file_descriptors = pollServerFD;
                        tempSocket.file_descriptors.events = POLLIN | POLLRDHUP;
                        tempSocket.last_record = time(NULL);

                        dpl_insert_at_index(connections, (void *) (&tempSocket), dpl_size(connections), true);
                    } else {
                        sensor_data_t incomingData;
                        tcp_receive(currenElement->socket_id, (void *) &incomingData.id,
                                    (int *) sizeof(incomingData.id));
                        currenElement->sensor_id = incomingData.id;

                        tcp_receive(currenElement->socket_id, (void *) &incomingData.value,
                                    (int *) sizeof(incomingData.value));

                        tcp_receive(currenElement->socket_id, (void *) &incomingData.ts,
                                    (int *) sizeof(incomingData.ts));

                        currenElement->last_record = time(NULL);
                        fprintf(fp, "%d %f %ld\n", incomingData.id, incomingData.value, incomingData.ts);
                    }
                } else if (currenElement->file_descriptors.revents == POLLRDHUP) {
                    tcp_close(&(currenElement->socket_id));
                    connections = dpl_remove_at_index(connections, i, true);
                    pollServer.last_record = time(NULL);
                    connectionCount = dpl_size(connections);
                    break;
                }

                if (connectionCount == 1 && (pollServer.last_record + TIMEOUT) < time(NULL)) {
                    tcp_close(&(currenElement->socket_id));
                    active = false;
                    connmgr_free();
                }
            }
        }
    }
    fclose(fp);
}

void connmgr_free() {
    dpl_free(&connections, true);
}
