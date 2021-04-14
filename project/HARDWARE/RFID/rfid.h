#ifndef _RFID_H_
#define _RFID_H_

#include "stm32f4xx.h" 
#include "sys.h"
#include "mfrc522.h"
#include "delay.h"
#include "includes.h"
#include "oled.h"

static u8  mfrc552pidbuf[18];
static u8  card_pydebuf[2];
static u8  card_numberbuf[5];
static u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
static u8  card_writebuf[16]={1,2,3,4,5,6,0,0,0,0,0,0,0,0,0,0};
static u8  card_readbuf[18];

extern void MFRC522Get(char *caidData);
extern void MFRC522Set(u8 *card_pwd);

#endif
