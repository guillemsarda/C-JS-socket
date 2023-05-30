#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include "../headers/hash.h"

/* BASE64 ENCODING:
This function takes the hashed accept-value and encodes it to produce the corresponding handshake's accept-value.
*/
void base64_encode(const unsigned char *hashed_accept_value, size_t hashed_accept_value_len, char *encoded_accept_value)
{
  BIO *b64 = BIO_new(BIO_f_base64());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

  BIO *bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);
  BIO_write(bio, hashed_accept_value, hashed_accept_value_len);
  BIO_flush(bio);

  BUF_MEM *buf_mem;
  BIO_get_mem_ptr(bio, &buf_mem);

  memcpy(encoded_accept_value, buf_mem->data, buf_mem->length);
  encoded_accept_value[buf_mem->length] = '\0';

  BIO_free_all(bio);
}

/* CREATE ACCEPT HASH */
char *create_accept_hash(char *req)
{
  char property[250];
  char hash_key[250] = "";
  char *hash_accept = malloc(sizeof(char) * 250);
  char handshake_key[250] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

  int i;
  for (i = 0; req[i] != '\0'; i++)
  {
    if (req[i] != '\r' && req[i] != '\n' && req[i] != ':')
    {
      strncat(property, &req[i], 1);
    }
    else if (strcmp(property, "Sec-WebSocket-Key") == 0)
    {
      break;
    }
    else
    {
      memset(property, 0, strlen(property));
    }
  }

  while (req[i] != '\r')
  {
    if (req[i] == ' ' || req[i] == ':')
    {
      i++;
      continue;
    }
    strncat(hash_key, &req[i], 1);
    i++;
  }

  strcat(hash_key, handshake_key);

  unsigned char digest[SHA_DIGEST_LENGTH];

  SHA1((const unsigned char *)hash_key, strlen(hash_key), digest);

  // Perform base 64 econding to the digest hashed value
  char *base64_encoded = malloc(sizeof(char) * 1024);
  printf("\nSIZE: %lu\n", sizeof(base64_encoded));
  base64_encode(digest, SHA_DIGEST_LENGTH, base64_encoded);

  return base64_encoded;
}