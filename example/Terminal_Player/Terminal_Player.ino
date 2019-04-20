#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3); // RX, TX

#define ShowSerial Serial 
#include "WT2003S_Player.h"


WT2003S Mp3Player;
uint8_t vol = 10;
uint32_t spi_flash_songs = 0;
uint32_t sd_songs = 0;
STROAGE workdisk = SD;
struct Play_history
{
    uint8_t disk;
    uint16_t index;
    char name[8];
} *SPISong,*SDSong;

void readSongName(struct Play_history* ph, uint32_t num, STROAGE disk){
    Mp3Player.volume(0);
    delay(100);
    switch (disk){
        case SPIFLASH:
        Mp3Player.playSPIFlashSong(0x0001);
        break;
        case SD:
        Mp3Player.playSDRootSong(0x0001);
        break;
        case UDISK:
        Mp3Player.playUDiskRootSong(0x0001);
        break;
    }
    ShowSerial.println("2...");
    for(int i = 0; i < num ; i++){
        delay(300);
        ph[i].disk = disk;
        ph[i].index = Mp3Player.getTracks();
        Mp3Player.getSongName(ph[i].name);
        Mp3Player.next();
    }
    ShowSerial.println("4...");
    Mp3Player.pause_or_play();
    Mp3Player.volume(14);
    delay(100);
}
void getAllSong(){
    uint8_t diskstatus = Mp3Player.getDiskStatus();
    ShowSerial.println(diskstatus);
    spi_flash_songs = Mp3Player.getSPIFlashMp3FileNumber();
    ShowSerial.print("SPIFlash:");
    ShowSerial.println(spi_flash_songs);
    if(spi_flash_songs > 0){
        SPISong = (struct Play_history*)malloc(spi_flash_songs * sizeof(struct Play_history));
        readSongName(SPISong,spi_flash_songs,SPIFLASH);
    }
    if(diskstatus && 0x01) { // have SD
        sd_songs = Mp3Player.getSDMp3FileNumber();
        ShowSerial.print("SD:");
        ShowSerial.println(sd_songs);
        if(sd_songs > 0){
            SDSong = (struct Play_history*)malloc(sd_songs * sizeof(struct Play_history));
            ShowSerial.println("1...");
            readSongName(SDSong,sd_songs,SD);
        }
    }
}
void printSongs(){
        ShowSerial.print("-------------------");
        ShowSerial.print("index");
        ShowSerial.print("<-------->");
        ShowSerial.print("name");
        ShowSerial.print("-------------------");
        ShowSerial.println();
        ShowSerial.println("-------------------spi flash-------------------------------");
    for(int i = 0 ; i < spi_flash_songs; i++){
        ShowSerial.print("-------------------");
        ShowSerial.print(SPISong[i].index);
        ShowSerial.print("<-------->");
        ShowSerial.print(SPISong[i].name);
        ShowSerial.print("-------------------");
        ShowSerial.println();
    }
     ShowSerial.println("-------------------sd card-------------------------------");
    for(int i = 0 ; i < sd_songs; i++){
        ShowSerial.print("-------------------");
        ShowSerial.print(SDSong[i].index);
        ShowSerial.print("<-------->");
        ShowSerial.print(SDSong[i].name);
        ShowSerial.print("-------------------");
        ShowSerial.println();
    }
}
void setup(){
    Serial.begin(9600);
    SSerial.begin(9600);
    ShowSerial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
    Mp3Player.init(SSerial);
    ShowSerial.println("0...");
    getAllSong();
    printMenu();
    printSongs();
}

void loop(){
    if(ShowSerial.available()){
        char cmd = ShowSerial.read();
        switch (cmd)
        {
        case '+':{
            ShowSerial.print("Volume up: ");
            vol = Mp3Player.getVolume();
            Mp3Player.volume(++vol);
            ShowSerial.print(vol);
            ShowSerial.println();
            break;
        }
        case '-':{
            ShowSerial.print("Volume down: ");
            vol = Mp3Player.getVolume();
            if(--vol > 31) vol = 0;
            Mp3Player.volume(vol);
            ShowSerial.print(vol);
            ShowSerial.println();            
            break;
        }
        case 't':{
            uint8_t status;
            ShowSerial.print("status:");
            status = Mp3Player.getStatus();
            if(status == 0x01) ShowSerial.print("playing"); 
            if(status == 0x02) ShowSerial.print("stop"); 
            if(status == 0x03) ShowSerial.print("pause"); 
            ShowSerial.println();  
            break;
        }
        case 'n':{
            Mp3Player.next();
            break;
        }      
        case 'p':{
            Mp3Player.pause_or_play();
            break;
        }   
        case 'w':{
            Mp3Player.playMode(SINGLE_SHOT);
            break;
        }  
        case 'x':{
            Mp3Player.playMode(SINGLE_CYCLE);
            break;
        }  
        case 'y':{
            Mp3Player.playMode(CYCLE);
            break;
        }  
        case 'z':{
            Mp3Player.playMode(RANDOM);
            break;
        }  
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ShowSerial.print("play:");
            if(workdisk == SD) {Mp3Player.playSDRootSong(cmd - '0'); ShowSerial.print(SDSong[cmd - '0'].name);}
            if(workdisk == SPIFLASH){ Mp3Player.playSPIFlashSong(cmd - '0'); ShowSerial.print(SPISong[cmd - '0'].name);}
            ShowSerial.println();  
            break;       
        default:
            break;
        }
    }
}

void printMenu( void )
{
  ShowSerial.println("MP3 Command List:");
  ShowSerial.println("-----------------");
  ShowSerial.println("'+' or '-'  : raise/lower volume");
  ShowSerial.println("'n'         : next song");
  ShowSerial.println("'s'         : switch play disk, spi flash");
  ShowSerial.println("'p'         : play or pause");
  ShowSerial.println("'w'         : set playmode single no loop");
  ShowSerial.println("'x'         : set playmode single loop");
  ShowSerial.println("'y'         : set playmode all loop");
  ShowSerial.println("'z'         : set playmode random");
  ShowSerial.println("'X'         : play the file that was copied to the SD card as the Xth file - you just type the number you want - (x in [1,9])");
  ShowSerial.println("             (Yes, this really does go by copy order.)");
  ShowSerial.println();
  ShowSerial.println("Any other key to show this menu");
  ShowSerial.println();
}
