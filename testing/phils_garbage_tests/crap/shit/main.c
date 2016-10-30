#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <thread.h>

#include <netinet/in.h>
#include <shell.h>

struct sockaddr_in getipa(const char *, int);

void *runclient(void *);
void *runserver(void *);

static char server_stack[THREAD_STACKSIZE_DEFAULT];
static char client_stack[THREAD_STACKSIZE_DEFAULT];

int main()
{
    kernel_pid_t server;
    kernel_pid_t client;

    server = thread_create(server_stack, sizeof(server_stack),
                           THREAD_PRIORITY_MAIN - 1,
                           THREAD_CREATE_STACKTEST,
                           &runserver,
                           NULL,
                           "server");

    client = thread_create(client_stack,
                           sizeof(client_stack),
                           THREAD_PRIORITY_MAIN - 1,
                           THREAD_CREATE_STACKTEST,
                           &runclient,
                           NULL,
                           "client");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, sizeof(line_buf));

    return EXIT_SUCCESS;
}

struct sockaddr_in getipa(const char *hostname, int port)
{
    struct sockaddr_in ipa;
    ipa.sin_family = AF_INET;
    ipa.sin_port = htons(port);

#if 0
    struct hostent *localhost = gethostbyname(hostname);
    if (!localhost) {
        printerror("resolveing localhost");

        return ipa;
    }

    char *addr = localhost->h_addr_list[0];
    memcpy(&ipa.sin_addr.s_addr, addr, sizeof addr);
#endif

    return ipa;
}

void *runserver(void *context)
{
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    if (sfd == -1) {
        fprintf(stderr, "could not create the socket\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    struct sockaddr_in isa = getipa("localhost", 1025);

    if (bind(sfd, (struct sockaddr *) &isa, sizeof isa) == -1) {
        fprintf(stderr, "couldnt bind socket\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    if (listen(sfd, 1) == -1) {
        fprintf(stderr, "socket failed to listen\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    int cfd = accept(sfd, NULL, NULL);

    if (cfd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            puts("SIGIO recieved for listen socket, but don't know why.");
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "failed to accept a connection\n");
            exit(EXIT_FAILURE);
            return NULL;
        }
    }

    char msg[] = "Message to be sent";

    if (send(cfd, (void *) msg, sizeof msg, 0) == -1) {
        fprintf(stderr, "failed to send message\n");
        exit(EXIT_FAILURE);
    }

    //shutdown(cfd, SHUT_RDWR);

    return NULL;
}

void *runclient(void *context)
{
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    if (sfd == -1) {
        fprintf(stderr, "failed to create socket in function: %s\n", __FUNCTION__);
        exit(EXIT_FAILURE);
        return NULL;
    }

    struct sockaddr_in isa = getipa("localhost", 1025);

    if (connect(sfd, (struct sockaddr *) &isa, sizeof isa) == -1) {
        fprintf(stderr, "connect didn't work.\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    char buff[255];
    ssize_t size = recv(sfd, (void *) buff, sizeof buff, 0);

    if (size == -1) {
        fprintf(stderr, "failed to receive data.\n");
        exit(EXIT_FAILURE);
    } else {

        buff[size] = '\0';

        puts(buff);
    }

    //shutdown(sfd, SHUT_RDWR);

    return NULL;
}
