#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../headers/socket.h"

/* CREATE SOCKET */
short socket_create(void)
{
  short h_socket;
  printf("Creating the socket...\n\n");
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