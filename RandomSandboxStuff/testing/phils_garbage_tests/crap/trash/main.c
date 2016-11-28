#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>

#define SERVER_BUFFER_SIZE 64
static char server_buffer[SERVER_BUFFER_SIZE];

static char thread_stack[THREAD_STACKSIZE_DEFAULT];

void launch_thread(in_port_t *port);

/*! the main function - the entry point of the application. */
int main(int argc, char *argv[])
{
    in_port_t port = 1234;

    launch_thread(&port);

    int a_socket;
    a_socket = socket(AF_INET6,
                      SOCK_DGRAM,
                      IPPROTO_UDP);

    if (a_socket < 0) {
        goto error;
    }

    socklen_t address_len;
    struct sockaddr_in6 sa = {
        .sin6_family = AF_INET6
    };

    memset(&sa.sin6_addr, 0, sizeof(sa.sin6_addr));

    sa.sin6_port = htons(port);

    address_len = SOCKADDR_MAX_DATA_LEN;

    int ret = bind(a_socket,
                   (struct sockaddr *) &sa,
                   sizeof(sa));
    if (ret < 0) {
        goto error;
    }

    struct sockaddr_in6 src;
    socklen_t src_len = sizeof(struct sockaddr_in6);
    ret = recvfrom(a_socket, server_buffer, sizeof(server_buffer),
                   0, (struct sockaddr *) &src, &src_len);

    if (ret < 0) {
        puts("Error on receive");
    } else if (ret == 0) {
        puts("peer's gone");
    } else {
        printf("got data: ");
        puts(server_buffer);
    }

    return EXIT_SUCCESS;

error:
    if (a_socket >= 0) {
        close(a_socket);
    }

    return EXIT_FAILURE;
} /* END of main */

static kernel_pid_t pid;

static void thd_fun(void *a) {
    in_port_t *port = (in_port_t *) a;
    if (port == NULL) {
        puts("error port in thd_fun was NULL");
        return;
    }

    struct sockaddr_in6 src, dst;
    static char const data[] = 
        "Test Text Is Here";

    int s;

    src.sin6_family = AF_INET6;
    memset(&src.sin6_addr, 0, sizeof(src.sin6_addr));
    src.sin6_port = htons(*port);

    memcpy(&dst, &src, sizeof(struct sockaddr_in6));

    s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    if (s < 0) {
        puts("failed to make socket");
        return;
    }

    usleep(5000 * 1000); /* sleep 5 seconds */

    if (sendto(s, data, sizeof(data) - 1, 0, (struct sockaddr *) &dst, sizeof(dst)) < 0) {
        puts("failed to send");
    } else {
        puts("send data");
    }

    close(s);
}

void launch_thread(in_port_t *port) {
    pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST,
                        &thd_fun,
                        port,
                        "thread");
}
