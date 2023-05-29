#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

void base64_encode(const unsigned char *hashed_accept_value, size_t hashed_accept_value_len, char *encoded_accept_value);

#endif