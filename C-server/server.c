#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include "./headers/hash.h"
// To perform a raw request we can use netcat (nc)

/* CREATE SOCKET */
short socket_create(void)
{
  short h_socket;
  printf("Create the socket\n");
  h_socket = socket(AF_INET, SOCK_STREAM, 0);
  return h_socket;
}

/* BIND SOCKET */
int bind_socket(int h_socket)
{
  int i_ret_val = -1;
  int client_port = 3000;

  struct sockaddr_in remote = {0};

  /* INTERNET ADDRESS FAMILY */
  remote.sin_family = AF_INET;

  /* ACCEPT? ANY INCOMING INTERFACE */
  remote.sin_addr.s_addr = INADDR_ANY;
  remote.sin_port = htons(client_port); /* LOCAL PORT */

  i_ret_val = bind(h_socket, (const struct sockaddr *)&remote, sizeof(remote));
  return i_ret_val;
}

int main(int argc, char *argv[])
{
  int socket_desc = 0, sock = 0, client_len = 0;
  struct sockaddr_in client;
  char client_message[200] = {0};
  char server_response[200] = {0};

  /* CREATE SOCKET */
  socket_desc = socket_create();
  if (socket_desc == -1)
  {
    perror("Could not create the socket...\n");
    return 1;
  }

  printf("Socket created! :D\n");

  /* BIND */
  if (bind_socket(socket_desc) < 0)
  {
    perror("Bind failed...\n");
    return 1;
  }

  printf("Socket bound!\n");

  /* ACCEPT INCOMING CONNECTION */
  printf("Waiting for connections...\n");
  client_len = sizeof(struct sockaddr_in);

  /* SERVER LISTENS */
  if (listen(socket_desc, 3) < 0)
  {
    perror("Couldn't start the server.\n");
    return 1;
  }

  while (1)
  {
    // Accept the incoming connection from a client
    sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&client_len);

    if (sock < 0)
    {
      perror("Couldn't accept client's request...\n");
      return 1;
    }

    printf("Connection accepted!\n");

    short receive_status = 1;
    receive_status = recv(sock, (void *)client_message, 200, 0);

    if (receive_status < 0)
    {
      perror("Couldn't receive client's message...\n");
      return 1;
    };
    printf("Raw client reply: %s\n\n", client_message);

    char *accept_key;
    accept_key = create_accept_hash(client_message);

    printf("\n");
    printf("STR ACCEPT KEY: %s", accept_key);
    printf("\n");
    // else if (send(sock, server_response, strlen(server_response), 0) < 0)

    char test_str[1024];

    snprintf(test_str, sizeof(test_str),
             "HTTP/1.1 101 Web Socket Protocol Handshake\r\nupgrade: websocket\r\nconnection: Upgrade\r\nsec-websocket-accept: %s\r\n\r\n",
             accept_key);

    printf("\nREQUEST: %s", test_str);

    if (send(sock, test_str, strlen(test_str), 0) < 0)
    {
      perror("Couldn't send: ");
      return 1;
    }

    close(sock);
    sleep(1);
  }
}
