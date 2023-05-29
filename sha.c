#include <stdio.h>
#include <stdlib.h>
// #include <openssl/evp.h>
#include <openssl/sha.h>

char hash_key[250] = "zVGCanc9qjrl+66s9W16dw==258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

/*
int main()
{
  EVP_MD_CTX *ctx;

  ctx = EVP_MD_CTX_create();

  if (ctx == NULL)
    return 1;

  if (EVP_DigestInit_ex(ctx, EVP_sha1(), NULL) != 1)
    return 1;

  if (EVP_DigestUpdate(ctx, hash_key, sizeof(hash_key)) != 1)
    return 1;

  unsigned char digest[EVP_MAX_MD_SIZE];
  unsigned int digest_len = sizeof(digest);

  if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1)
    return 1;

  for (int i = 0; i < digest_len; i++)
    printf("%02x", digest[i]);

  EVP_MD_CTX_destroy(ctx);
}
*/

int main()
{
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA_CTX context;
  SHA1_Init(&context); // Initialize the hash
  SHA1_Update(&context, hash_key, sizeof hash_key);
  SHA1_Final(md, &context);
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    printf("%02x", md[i]);
}
