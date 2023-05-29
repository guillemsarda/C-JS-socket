#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include <string.h>
#include "../headers/base64.h"

/* BASE64 ENCODING:
This function takes the hashed accept value and encodes it to produce the corresponding handshake's accept value.
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