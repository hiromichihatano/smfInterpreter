/*
 * main.c
 *
 *  Created on: 2016/10/23
 *      Author: hiromichihatano
 */

#include <stdio.h>
#include <stdlib.h>
#include "smf.h"

#define BUFLEN (1024*128)	/* 128KB */


/**
 * バイナリを 16bytes ずつオフセット付きで表示する
 */
void showBinary(uint8_t buf[], int32_t len)
{
	int32_t i;

	for(i=0; i<len; i++){
		if(i % 16 == 0){
			printf("\n%08x: ", i);
		}
		printf("%02x ", buf[i]);
	}
	puts("");
}



int main(int argc, char *argv[])
{
	uint8_t smfBuf[BUFLEN];
	int ch;
	int32_t smfLen, i;
	FILE *fp;
	timebase_t timebase = 0;

	smfInfo smfi;

	if (argc < 2) {
		printf("usage: %s <SMF FileName>\n", argv[0]);
		return -1;
	}

	fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		printf("file %s open failed.\n", argv[1]);
		return -2;
	}

	for(i=0; i<BUFLEN; i++){
		ch = fgetc(fp);
		if(ch == EOF) {
			break;
		}
		smfBuf[i] = (uint8_t)ch;
	}

	smfLen = i-1;
	//showBinary(smfBuf, smfLen);
	printf("length is dec:%d, hex:%x\n", smfLen, smfLen);
	smfLibInterpreterInit(&smfi, smfBuf, smfLen);

	while(timebase < 51700) {
		smfMidiEventTimerTick(&smfi, timebase);

		usleep(100 * 1000);
		//timebase += _usToTimebase(100*1000, smfi.smfTimeDivision, smfi.smfTempo);
		timebase += smfLibUsToTimebase(1000*1000, smfi.smfTimeDivision, smfi.smfTempo);
		fflush(stdout);
	}

	return 0;
}
