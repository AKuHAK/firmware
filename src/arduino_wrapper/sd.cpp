#include "config.h"

#include "SdFat.h"
#include "SPI.h"

#include "hardware/gpio.h"

extern "C" {
#include "debug.h"
}

#include <stdio.h>

#define NUM_FILES 8

static SdFat sd;
static File files[NUM_FILES];

extern "C" void sd_init() {
    SPI1.setRX(SD_MISO);
    SPI1.setTX(SD_MOSI);
    SPI1.setSCK(SD_SCK);
    SPI1.setCS(SD_CS);

    int ret = sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_BAUD, &SPI1));
    if (ret != 1)
        fatal("failed to mount the card");
}

void sdCsInit(SdCsPin_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, 1);
}

void sdCsWrite(SdCsPin_t pin, bool level) {
    gpio_put(pin, level);
}

extern "C" int sd_open(const char *path, int oflag) {
    size_t fd;

    for (fd = 0; fd < NUM_FILES; ++fd)
        if (!files[fd].isOpen())
            break;

    /* no fd available */
    if (fd >= NUM_FILES)
        return -1;

    files[fd].open(path, oflag);

    /* error during opening file */
    if (!files[fd].isOpen())
        return -1;

    return fd;
}

#define CHECK_FD(fd) if (fd >= NUM_FILES || !files[fd].isOpen()) return -1;
#define CHECK_FD_VOID(fd) if (fd >= NUM_FILES || !files[fd].isOpen()) return;

extern "C" void sd_close(int fd) {
    CHECK_FD_VOID(fd);

    files[fd].close();
}

extern "C" void sd_flush(int fd) {
    CHECK_FD_VOID(fd);

    files[fd].flush();
}

extern "C" int sd_read(int fd, void *buf, size_t count) {
    CHECK_FD(fd);

    return files[fd].read(buf, count);
}

extern "C" int sd_write(int fd, void *buf, size_t count) {
    CHECK_FD(fd);

    return files[fd].write(buf, count);
}

extern "C" int sd_seek(int fd, uint64_t pos) {
    CHECK_FD(fd);

    /* return 1 on error */
    return files[fd].seekSet(pos) != true;
}

extern "C" int sd_mkdir(const char *path) {
    /* return 1 on error */
    return sd.mkdir(path) != true;
}

extern "C" int sd_exists(const char *path) {
    return sd.exists(path);
}
