#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

void base64_encode(const unsigned char *hashed_accept_value, size_t hashed_accept_value_len, char *encoded_accept_value);
char *create_accept_hash(char *req);

#endif