/**
    The MIT License (MIT)

    Author: Jiaxuan Weng (jiaxuan.weng@outlook.com)

    Copyright (C) 2023  Seeed Technology Co.,Ltd.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef __WT2605C__
#define __WT2605C__

#include "Arduino.h"

#define AT_HEADER          "AT+"
#define AT_CMD_PLAY        "PLAY"
#define AT_CMD_LPLAY       "LPLAY"
#define AT_CMD_SPLAY       "SPLAY"
#define AT_CMD_FPLAY       "FPLAY"
#define AT_CMD_PP          "PP"
#define AT_CMD_STOP        "STOP"
#define AT_CMD_NEXT        "NEXT"
#define AT_CMD_PREV        "PREV"
#define AT_CMD_VOL         "VOL"
#define AT_CMD_VOLUP       "VOLUP"
#define AT_CMD_VOLDOWN     "VOLDOWN"
#define AT_CMD_REPEATMODE  "REPEATMODE"
#define AT_CMD_STEPINPLAY  "STEPINPLAY"
#define AT_CMD_TOTALTIME   "TOTALTIME"
#define AT_CMD_CURTIME     "CURTIME"
#define AT_CMD_CHANGE_DEV  "CHANGE_DEV"
#define AT_CMD_BUSY        "BUSY"
#define AT_CMD_VBATPCT     "VBATPCT"
#define AT_CMD_GVER        "GVER"
#define AT_CMD_GCFGVER     "GCFGVER"
#define AT_CMD_POWEROFF    "POWEROFF"
#define AT_CMD_OTA         "OTA"
#define AT_CMD_COPY        "COPY"
#define AT_CMD_BAUD        "BAUD"

#define WT2605C_TIMEOUT       1000
#define WT2605C_SEND_MAX_SIZE 64

#define STORAGE_SPIFLASH   "fat_nor"
#define STORAGE_SD         "sd0"
#define STORAGE_UDISK      "udisk0"

typedef enum {
    WT2605C_CYCLE        = 0x00,
    WT2605C_SINGLE_CYCLE = 0x01,
    WT2605C_DIR_CYCLE    = 0x02,
    WT2605C_RANDOM       = 0x03,
    WT2605C_SINGLE_SHOT  = 0x04,
} WT2605C_PLAY_MODE;

typedef enum {
    WT2605C_SPIFLASH = 0x00,
    WT2605C_SD       = 0x01,
    WT2605C_UDISK    = 0x02,
} WT2605C_STORAGE;


template <class T>
class WT2605C {
private:
    T* _serial;
    uint8_t _busyPin;
    uint8_t getResult();
    String getStorageName(WT2605C_STORAGE storage);

public:
    WT2605C();
    void init(T& serialPort);
    void init(T& serialPort, uint8_t pin);

    uint8_t playSPIFlashSong(uint16_t index);
    uint8_t playSDRootSong(uint32_t index);
    uint8_t playSDSong(const char* fileName);
    uint8_t playSDDirectorySong(const char* dir, uint16_t index);
    uint8_t playUDiskRootSong(uint32_t index);
    uint8_t playUDiskSong(const char* fileName);
    uint8_t playUDiskDirectorySong(const char* dir, uint32_t index);
    uint8_t pause_or_play();
    uint8_t stop();
    uint8_t next();
    uint8_t previous();
    uint8_t volume(uint8_t vol);
    uint8_t volumeDown();
    uint8_t volumeUp();
    uint8_t playMode(WT2605C_PLAY_MODE mode);
    uint8_t cutInPlay(WT2605C_STORAGE device, uint32_t index);
    // uint8_t copySDtoSPIFlash();
    // uint8_t copyUDisktoSPIFlash();
    // uint8_t writeUserData(uint16_t address,  uint32_t data);
    // uint8_t switchWorkDisk(WT2605C_STORAGE disk);

    // int8_t getVolume();
    // int8_t getStatus();
    // uint32_t getSPIFlashMp3FileNumber();
    // uint32_t getSDMp3FileNumber();
    // uint32_t getSDDirectoryMp3FileNumber(const char* dir);
    // uint32_t getUDiskMp3FileNumber();
    // uint32_t getUDiskDirectoryMp3FileNumber(const char* dir);
    // uint32_t getTracks();
    // void getSongName(char* Songname);
    // uint8_t getDiskStatus();
    // void getSPIFLashMp3Data(char* data, uint16_t address, uint16_t len);
};

#endif
