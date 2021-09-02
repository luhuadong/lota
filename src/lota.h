#ifndef __LOTA_H
#define __LOTA_H

#include <stddef.h>
#include <stdint.h>

int lota_download(const char *url, const char *save_path);

int check_md5_string(unsigned char *dest_str, unsigned int dest_len, char *md5_str);
int check_md5_file(const char *file_path, char *md5_str);

#endif /* __LOTA_H */