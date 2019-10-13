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
#include "sample.h"

#define BUFLEN (1024*1024)	/* 1024KB */

static uint8_t _smfBuf[BUFLEN];

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

/*
 * 引数を解析し、ファイルの内容を _smfBuf に格納する
 * 読み込んだファイル長を返す。エラーの場合は 0 未満の値を返す
 */
static int32_t _parse_arguments(int argc, char *argv[])
{
	int16_t ch;
	int32_t i;

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
		ch = (int16_t)fgetc(fp);
		if(ch == EOF) {
			break;
		}
		_smfBuf[i] = (uint8_t)ch;
	}

	fclose(fp);
	return i-1; // EOF 分をデクリメント
}

int main(int argc, char *argv[])
{
	const int TICK_US = 10 * 1000; // us
	int32_t smfLen, i;

	smfInfo smfi;
	smf_callback_t smfcb;

	smfLen = _parse_arguments(argc, argv);
	if (smfLen < 0) {
		exit(smfLen);
	}

	//showBinary(_smfBuf, smfLen);
	printf("length is dec:%d, hex:%x\n", smfLen, smfLen);

	// デバッグ用関数の登録
	smfDbgRegisterLogFunc(SMFLOG_INFO, vprintf);
	smfDbgRegisterLogFunc(SMFLOG_STD, vprintf);
	smfDbgRegisterLogFunc(SMFLOG_ERR, vprintf);

	// SMF の初期化
	smfLibInterpreterInit(&smfi, _smfBuf, smfLen);

	// Callback 関数の登録
	smfsample_set_callbacks(&smfcb);

	timebase_t timebase = 0;
	while(timebase < 51700) {
		smfLibTimerTick(&smfi, &smfcb, timebase);
		usleep(TICK_US);
		timebase += smfLibUsToTimebase(TICK_US, smfi.smfTimeDivision, smfi.smfTempo);
	}

	return 0;
}