#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

int checksum_add(void *buf, size_t len);
int checksum_valid(void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif
