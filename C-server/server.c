#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "./headers/hash.h"
#include "./headers/socket.h"

int main(int argc, char *argv[])
{
  int socket_desc = 0, sock = 0, client_len = 0, is_handshake = 0;
  struct sockaddr_in client;
  char client_message[1024];
  char server_response[1024];

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

  // Accept the incoming connection from a client
  sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&client_len);

  if (sock < 0)
  {
    perror("Couldn't accept client's request...\n");
    return 1;
  }

  printf("Connection accepted!\n\n");

  while (1)
  {
    printf("Waiting for a message...\n\n");
    short receive_status = 1;
    receive_status = recv(sock, (void *)client_message, 1024, 0);

    if (receive_status < 0)
    {
      perror("Couldn't receive client's message...\n");
      return 1;
    };
    printf("Raw client reply: \n%s\n\n", client_message);

    char *accept_key;
    if (is_handshake == 0)
    {
      accept_key = create_accept_hash(client_message);

      snprintf(server_response, sizeof(server_response),
               "HTTP/1.1 101 Web Socket Protocol Handshake\r\nupgrade: websocket\r\nconnection: Upgrade\r\nsec-websocket-accept: %s\r\n\r\n",
               accept_key);
      if (send(sock, server_response, strlen(server_response), 0) < 0)
      {
        perror("Couldn't send: ");
        return 1;
      }
      is_handshake++;
    }
    else
    {
      strcpy(server_response, "Hello again");
      if (send(sock, server_response, strlen(server_response), 0) < 0)
      {
        perror("Couldn't send: ");
        return 1;
      }
    }
  }
  close(sock);
}
