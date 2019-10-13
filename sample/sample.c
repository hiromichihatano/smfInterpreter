/*
 * main.c
 *
 *  Created on: 2016/10/23
 *      Author: hiromichihatano
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "smf.h"

#define BUFLEN (1024*1024)	/* 1024KB */


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
	const int TICK_US = 10 * 1000; // us
	uint8_t smfBuf[BUFLEN];
	int ch;
	int32_t smfLen, i;

	smfInfo smfi;

	if (argc < 2) {
		printf("usage: %s <SMF FileName>\n", argv[0]);
		return -1;
	}

	FILE *fp = fopen(argv[1], "rb"); // MIDI(SMF) ファイルの読込み
	if (fp == NULL) {
		printf("file %s open failed.\n", argv[1]);
		return -2;
	}

	for(i=0; i<BUFLEN; i++){ // バイト配列に格納
		ch = fgetc(fp);
		if(ch == EOF) {
			break;
		}
		smfBuf[i] = (uint8_t)ch;
	}

	smfLen = i-1;
	//showBinary(smfBuf, smfLen);
	printf("length is dec:%d, hex:%x\n", smfLen, smfLen);

	// デバッグ用関数の登録
	smfDbgRegisterLogFunc(SMFLOG_INFO, vprintf);
	smfDbgRegisterLogFunc(SMFLOG_STD, vprintf);
	smfDbgRegisterLogFunc(SMFLOG_ERR, vprintf);

	// SMF の初期化
	smfLibInterpreterInit(&smfi, smfBuf, smfLen);

	timebase_t timebase = 0;
	uint64_t time = 0u;
	while(timebase < 51700) {
		smfMidiEventTimerTick(&smfi, timebase);
		usleep(TICK_US);
		//timebase += _usToTimebase(100*1000, smfi.smfTimeDivision, smfi.smfTempo);
		timebase += smfLibUsToTimebase(TICK_US, smfi.smfTimeDivision, smfi.smfTempo);
		time += TICK_US;
		// printf("timebase %5d, time=%08d(us)\n", timebase, time);
		fflush(stdout);
	}

	return 0;
}
