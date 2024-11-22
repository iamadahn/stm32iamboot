#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "checksum.h"

static unsigned int checksum_calculate(void *buf, size_t len);

static unsigned int
checksum_calculate(void *buf, size_t len) {
    unsigned int ret = 0;

    if (buf == NULL) {
        return ret;
    }

    unsigned char* msg = (unsigned char*)buf;
    unsigned char ck_a = 0, ck_b = 0;

    for (size_t i = 0; i < len - 2; ++i) {
        ck_a += msg[i];
        ck_b += ck_a;
    }

    ret = (ck_a << 8) | ck_b;

    return ret;
}

int
checksum_add(void *buf, size_t len) {
    if (buf == NULL) {
        return 1;
    }

    unsigned char* msg = (unsigned char*)buf;

    unsigned int checksum = checksum_calculate(msg, len);

    if (checksum == 0) {
        return 1;
    }

    msg[len - 1] = checksum & 0x00FF;
    msg[len - 2] = (checksum >> 8) & 0x00FF;

    return 0;
}

int
checksum_valid(void *buf, size_t len) {
    if (buf == NULL) {
        return 1;
    }

    unsigned char* msg = (unsigned char*)buf;

    unsigned int checksum = checksum_calculate(msg, len);

    if (checksum == 0) {
        return 1;
    }

    unsigned char ck_a = msg[len-2], ck_b = msg[len-1];

    if ((ck_b != (checksum & 0x00FF)) & (ck_a != ((checksum >> 8) & 0x00FF))) {
        return 1;
    }

    return 0;
}

#pragma GCC pop_options
