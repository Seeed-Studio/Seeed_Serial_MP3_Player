/**
    The MIT License (MIT)

    Author: Jiaxuan Weng (jiaxuan.weng@outlook.com)

    Copyright (C) 2019  Seeed Technology Co.,Ltd.

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
#include "WT2605C_Player.h"
#include <String.h>

template <class T>
WT2605C<T>::WT2605C() {
    _busyPin = -1;
}

template <class T>
void WT2605C<T>::init(T& serialPort) {
    _serial = &serialPort;
}

template <class T>
void WT2605C<T>::init(T& serialPort, uint8_t pin) {
    _serial = &serialPort;
    _busyPin = pin;
}

template <class T>
String WT2605C<T>::getStorageName(STORAGE storage) {
    switch (storage) {
        case SPIFLASH:
            return STORAGE_SPIFLASH;
        case SD:
            return STORAGE_SD;
        case UDISK:
            return STORAGE_UDISK;
        default:
            return "";
    }
}

template <class T>
uint8_t WT2605C<T>::getResult(void) {
    String result = _serial->readString();
    if (result == "OK") {
        return 0;
    }
    return -1;
}

template <class T>
uint8_t WT2605C<T>::playSPIFlashSong(uint16_t index) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_SPIFLASH ",") + String((uint32_t)index);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playSDRootSong(uint32_t index) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_SD ",") + String((uint32_t)index);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playSDSong(const char* fileName) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_SD ",") + String(fileName);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playSDDirectorySong(const char* dir, uint16_t index) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_SD ",") + String(dir) + 
                    String(",") + String((uint32_t)index);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playUDiskRootSong(uint32_t index) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_UDISK ",") + String((uint32_t)index);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playUDiskSong(const char* fileName) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_UDISK ",") + String(fileName);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::playUDiskDirectorySong(const char* dir, uint32_t index) {
    String cmd = String(AT_HEADER AT_CMD_PLAY "=" STORAGE_UDISK ",")+ String(dir) + 
                    String(",") + String((uint32_t)index);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::pause_or_play() {
    String cmd = String(AT_HEADER AT_CMD_PP);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::stop() {
    String cmd = String(AT_HEADER AT_CMD_STOP);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::next() {
    String cmd = String(AT_HEADER AT_CMD_NEXT);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::previous() {
    String cmd = String(AT_HEADER AT_CMD_PREV);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::volume(uint8_t vol) {
    if (vol > 31) {
        vol = 32;
    }
    String cmd = String(AT_HEADER AT_CMD_VOL "=") + String((uint32_t)vol);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::volumeDown() {
    String cmd = String(AT_HEADER AT_CMD_VOLDOWN);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::volumeUp() {
    String cmd = String(AT_HEADER AT_CMD_VOLUP);
    _serial->println(cmd);
    return getResult();

}

template <class T>
uint8_t WT2605C<T>::playMode(PLAY_MODE mode) {
    String cmd = String(AT_HEADER AT_CMD_REPEATMODE "=") + String(((uint32_t)mode)+1);
    _serial->println(cmd);
    return getResult();
}

template <class T>
uint8_t WT2605C<T>::cutInPlay(STORAGE device, uint32_t index) {
    String cmd = String(AT_HEADER AT_CMD_STEPINPLAY "=") + getStorageName(device) + 
                    String(",") + String(index);
    _serial->println(cmd);
    return getResult();
}

/****************************************************************
    Function Name: copySDtoSPIFlash
    Description: Copy SD to SPI-FLASH
    Parameters: none
    Return:    0x00, mp3 file copy ok, config data copy ok
              0x01, mp3 file copy ok, config  data error
              0x02, mp3 file copy error. config data ok , maybe mp3 file too big to copy
              0x03, mp3 file cope ok, config data error, maybe mp3 file too big to copy
****************************************************************/
// template <class T>
// uint8_t WT2605C<T>::copySDtoSPIFlash() {
//     commandBytes[0] = WT2605C_COPY_SDTOSPIFLASH;
//     return sendCommand(1);
// }

// template <class T>
// uint8_t WT2605C<T>::copyUDisktoSPIFlash() {
//     commandBytes[0] = WT2605C_COPY_UDISKTOSPIFLASH;
//     return sendCommand(1);
// }

// /****************************************************************
//     Function Name: switchWorkDisk
//     Description: switch the work disk
//     Parameters: device:UDISK,SD,SPIFLASH
//     Return: >=0:return value -1:fail
// ****************************************************************/
// template <class T>
// uint8_t WT2605C<T>::switchWorkDisk(STORAGE disk) {
//     String cmd = String(AT_HEADER AT_CMD_CHANGE_DEV "=") + getStorageName(mode);
//     _serial->println(cmd);
//     return getResult();
// }

// /****************************************************************
//     Function Name: getVolume
//     Description: get the current volume
//     Parameters: none
//     Return: vol:the value of volume:0-30
// ****************************************************************/
// template <class T>
// int8_t WT2605C<T>::getVolume() {
//     uint8_t vol;
//     commandBytes[0] = WT2605C_GET_VOLUME;
//     sendCommand(1, &vol, 2);
//     return vol;
// }

// /****************************************************************
//     Function Name: getStatus
//     Description: get the current Status
//     Parameters: none
//     Return: status:play(01), stop(02), pause(03)
// ****************************************************************/
// template <class T>
// int8_t WT2605C<T>::getStatus() {
//     uint8_t status;
//     commandBytes[0] = WT2605C_GET_STATE;
//     sendCommand(1, &status, 2);
//     return status;
// }

// /****************************************************************
//     Function Name: getSPIFlashMp3FileNumber
//     Description: Query the total number of music files in SPIFLASH.
//     Parameters: none
//     Return: num:the total number of music files in SPIFLAS
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getSPIFlashMp3FileNumber() {
//     uint8_t num;
//     commandBytes[0] = WT2605C_GET_SPIFLASH_SONGCOUNT;
//     sendCommand(1, &num, 2);
//     return num;
// }

// /****************************************************************
//     Function Name: getSDMp3FileNumber
//     Description: Query the total number of music files in SD.
//     Parameters: none
//     Return: num:the total number of music files in SD
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getSDMp3FileNumber() {
//     union {
//         uint8_t d[2];
//         uint16_t n;
//     } num;
//     uint8_t tmp;
//     commandBytes[0] = WT2605C_GET_SD_SONGCOUNT;
//     sendCommand(1, num.d, 3);
//     tmp = num.d[0];
//     num.d[0] = num.d[1];
//     num.d[1] = tmp;
//     return num.n;
// }

// /****************************************************************
//     Function Name: getSDDirectoryMp3FileNumber
//     Description: Query the total number of music files in specified directory of SD.
//     Parameters: dir: specified directory name
//     Return: num:the total number of music files in specified directory of SD.
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getSDDirectoryMp3FileNumber(const char* dir) {
//     uint8_t num;
//     commandBytes[0] = WT2605C_GET_SD_SONGS_IN_FOLDER_COUNT;
//     commandBytes[1] = dir[0];
//     commandBytes[2] = dir[1];
//     commandBytes[3] = dir[2];
//     commandBytes[4] = dir[3];
//     commandBytes[5] = dir[4];
//     sendCommand(6, &num, 2);
//     return num;
// }

// /****************************************************************
//     Function Name: getUDiskMp3FileNumber
//     Description: Query the total number of music files in UDISK.
//     Parameters: none
//     Return: num:the total number of music files in SD
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getUDiskMp3FileNumber() {
//     uint8_t num;
//     commandBytes[0] = WT2605C_GET_UDISK_SONGCOUNT;
//     sendCommand(1, &num, 2);
//     return num;
// }

// /****************************************************************
//     Function Name: getUDiskDirectoryMp3FileNumber
//     Description: Query the total number of music files in specified directory of Disk.
//     Parameters: dir: specified directory name
//     Return: num:the total number of music files in specified directory of Disk.
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getUDiskDirectoryMp3FileNumber(const char* dir) {
//     uint8_t num;
//     commandBytes[0] = WT2605C_GET_UDISK_SONGS_IN_FOLDER_COUNT;
//     commandBytes[1] = dir[0];
//     commandBytes[2] = dir[1];
//     commandBytes[3] = dir[2];
//     commandBytes[4] = dir[3];
//     commandBytes[5] = dir[4];
//     sendCommand(6, &num, 2);
//     return num;
// }


// /****************************************************************
//     Function Name: getTracks
//     Description:Query the currently playing tracks
//     Parameters: none
//     Return: index:the music file index
// ****************************************************************/
// template <class T>
// uint32_t WT2605C<T>::getTracks() {
//     union {
//         uint8_t d[2];
//         uint16_t n;
//     } data;
//     uint8_t tmp;
//     commandBytes[0] = WT2605C_GET_FILE_PLAYING;
//     sendCommand(1, data.d, 3);
//     tmp = data.d[0];
//     data.d[0] = data.d[1];
//     data.d[1] = tmp;
//     return data.n;
// }

// /****************************************************************
//     Function Name: getSongName
//     Description:Query the song name of the currently playing song
//     Parameters: *Songname the pointer of Songname
//     Return: none
// ****************************************************************/
// template <class T>
// void WT2605C<T>::getSongName(char* Songname) {
//     commandBytes[0] = WT2605C_GET_SONG_NAME_PLAYING;
//     sendCommand(1, (uint8_t*)Songname, 9);
// }

// /****************************************************************
//     Function Name: getDiskStatus
//     Description: Query the current peripheral connection status
//     Parameters: none
//     Return: 0X01: no PC connection (BIT3=0), no U disk (BIT2=0), no SD card (BIT1=0), spi-flash (BIT0=1);
// 		   0X07: no PC connection (BIT3=0), U disk (BIT2=1), SD card (BIT1=1), spi-flash (BIT0=1);
// ****************************************************************/
// template <class T>
// uint8_t WT2605C<T>::getDiskStatus() {
//     uint8_t disk;
//     commandBytes[0] = WT2605C_DISKSTATUS;
//     sendCommand(1, &disk, 2);
//     return disk;
// }

// /****************************************************************
//     Function Name: getSPIFLashMp3Data
//     Description:Read user-cached data at the specified address (CF) (just read config in Flash)
//     Parameters: *data the pointer of data
//     Return: none
// ****************************************************************/
// template <class T>
// void WT2605C<T>::getSPIFLashMp3Data(char* data, uint16_t address, uint16_t len) {
//     if (len > 512) {
//         len = 512;
//     }
//     commandBytes[0] = WT2605C_GET_USERDATA;
//     commandBytes[1] = 0x00;
//     commandBytes[2] = (address >> 8) & 0xff;
//     commandBytes[3] = 0xff & address;

//     commandBytes[4] = (len >> 8) & 0xff;
//     commandBytes[5] = 0xff & len;
//     sendCommand(6, (uint8_t*)data, len + 4);
// }
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
template class WT2605C<Uart>;
#endif
template class WT2605C<HardwareSerial>;


#include <SoftwareSerial.h>
template class WT2605C<SoftwareSerial>;
