/*
 * smf_lib.c
 *
 *  Created on: 2016/10/24
 *      Author: hiromichihatano
 */

#include "smf.h"
#include "smf_lib.h"



const int32_t midiNoteFreq[] = {
    8, 9, 9, 10, 10, 11, 12, 12, 13,
    14, 15, 15, 16, 17, 18, 19, 21, 22, 23, 24, 26,
    28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52,
    55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104,
    110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208,
    220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415,
    440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831,
    880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661,
    1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322,
    3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645,
    7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544,
};

const char midiNoteName[][5] = {
    "C -2", "C#-2", "D -2", "D#-2", "E -2", "F -2", "F#-2", "G -2", "G#-2",
    "A -1", "A#-1", "B -1", "C -1", "C#-1", "D -1", "D#-1", "E -1", "F -1", "F#-1", "G -1", "G#-1",
    "A  0", "A# 0", "B  0", "C  0", "C# 0", "D  0", "D# 0", "E  0", "F  0", "F# 0", "G  0", "G# 0",
    "A  1", "A# 1", "B  1", "C  1", "C# 1", "D  1", "D# 1", "E  1", "F  1", "F# 1", "G  1", "G# 1",
    "A  2", "A# 2", "B  2", "C  2", "C# 2", "D  2", "D# 2", "E  2", "F  2", "F# 2", "G  2", "G# 2",
    "A  3", "A# 3", "B  3", "C  3", "C# 3", "D  3", "D# 3", "E  3", "F  3", "F# 3", "G  3", "G# 3",
    "A  4", "A# 4", "B  4", "C  4", "C# 4", "D  4", "D# 4", "E  4", "F  4", "F# 4", "G  4", "G# 4",
    "A  5", "A# 5", "B  5", "C  5", "C# 5", "D  5", "D# 5", "E  5", "F  5", "F# 5", "G  5", "G# 5",
    "A  6", "A# 6", "B  6", "C  6", "C# 6", "D  6", "D# 6", "E  6", "F  6", "F# 6", "G  6", "G# 6",
    "A  7", "A# 7", "B  7", "C  7", "C# 7", "D  7", "D# 7", "E  7", "F  7", "F# 7", "G  7",
};


/**
 * マイクロ秒を、タイムベース(I16Q16)値に変換する。
 *
 * @param timeUs  変換する時間(マイクロ秒)
 * @param timeDiv 時間単位。全音符辺りのタイムベース数
 * @param tempo   テンポ。４分音符辺りの時間(us)を示しているので注意。
 * @return 変換したタイムベース値。
 */
timebaseI24Q8_t smfLibUsToTimebase(int32_t timeUs, int32_t timeDiv, int32_t tempo)
{

	/** <時間(us)> * <全音符辺りのタイムベース数(tb)>  / <全音符の時間(us)> */
	//return timeUs * timeDiv / (4 * tempo);
	// return (timeUs * timeDiv) / (tempo >> 8);
	return (timeUs * timeDiv) / tempo;
}


uint32_t smfLibGetSmfFixedLe7(const uint8_t smfBuf[], off_t off, len_t len)
{
	int32_t i;
	uint32_t ret = 0;

	for(i=len-1; 0<=i; i--){
		ret <<= 7;
		ret += smfBuf[off + i] & 0x7f;
	}

	return ret;
}


uint32_t smfLibGetSmfFixedBe8(const uint8_t smfBuf[], off_t off, len_t len)
{
	int32_t i;
	uint32_t ret = 0;

	for(i=0; i<len; i++){
		ret <<= 8;
		ret += smfBuf[off + i];
	}

	return ret;
}


len_t smfLibGetSmfVar(const uint8_t smfBuf[], off_t off, int32_t *value)
{
	int32_t i;
	int32_t retVal = 0, ret=-1;


	for(i=0; i<4; i++) {
		retVal <<=7;
		retVal += smfBuf[off + i] & 0x7f;

		if(smfBuf[off + i] <= 0x7f) {
			ret = i+1;
			break;
		}
	}
	if(ret < 0) {
		return -1;
	}

	if(value != NULL) {
		*value = retVal;
	}

	return ret;
}


void smfLibRewindToStart(smfInfo *smfi)
{
	for(int32_t i=0; i<smfi->smfNrTracks; i++){
		int32_t initOffset = smfi->tracki[i].bodyOffset;
		len_t initEventTime;
		smfLibGetSmfVar(smfi->smfDataBuf, initOffset, &initEventTime);
		smfi->tracki[i].nextEventOffset = initOffset;
		smfi->tracki[i].nextEventTime = initEventTime;
	}
}


int32_t smfLibInterpreterInit(smfInfo *smfi, const uint8_t buf[], int32_t bufLen)
{
	off_t offset = 0;
	len_t len;
	int32_t i;
	smfTrackInfo *tracki;

	smfDbgPrintLog(SMFLOG_STD, "smfInterPreterInit starting...\n");
	smfi->smfDataBuf = buf;

	offset = smfInterpreterHeaderInit(smfi, buf, offset);

	for(i=0; i < smfi->smfNrTracks; i++) {
		tracki = &(smfi->tracki[i]);
		offset = smfInterpreterTrackInit(tracki, buf, offset);
		if(offset < 0) {
			smfDbgPrintLog(SMFLOG_ERR, "SMF Track %d Init Failed.\n", i);
			return -1;
		}
	}

	smfDbgPrintLog(SMFLOG_ERR, "smfInterPreterInit done.\n");
	return 0;
}

/**
 * @brief MIDI を 1tick 分再生を進める
 * 
 * @param smfi SMF 情報
 * @param smfcb SMF コールバック関数情報
 * @param time 関数呼び出し時の再生時間(呼び出し元で時間を調整すること)
 * @return ステータス(0:再生中 / 1:再生終了)
 * 
 * @note time が timebase_t 単位であることに注意すること
 *       値は smfLibUsToTimebase で usec -> timebase_t に変換できる
 */
int32_t smfLibTimerTick(smfInfo *smfi, const smf_callback_t *smfcb, timebase_t time) {
	return smfMidiEventTimerTick(smfi, smfcb, time);
}