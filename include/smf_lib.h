/*
 * smf_lib.h
 *
 *  Created on: 2016/10/24
 *      Author: hiromichihatano
 */

#ifndef SMF_LIB_H_
#define SMF_LIB_H_

#include <stdint.h>

//typedef int32_t size_t;
typedef int32_t off_t;
typedef int32_t len_t;
typedef int32_t timebase_t;
typedef int32_t timebaseI24Q8_t;

#define MAXTRACKS (20)

#ifndef UNUSED
	#define UNUSED(x) ((void)(x))
#endif

typedef struct {
	off_t bodyOffset;			/** MIDIデータ本体のオフセット */
	len_t endOffset;			/** MIDI body の長さ */
	off_t nextEventOffset;		/** 次に読むMIDIイベントの先頭を指す */
	timebase_t nextEventTime;	/** 次のイベント時間(積算時間) */
	uint8_t prevStateByte;		/** 前のイベントの statusByte */

} smfTrackInfo;

typedef struct {
	const uint8_t *smfDataBuf;
	int32_t smfFormat;
	int32_t smfNrTracks;
	int32_t smfTimeDivision;
	int32_t smfTempo;
	uint8_t prevStatusByte;
	smfTrackInfo tracki[MAXTRACKS];
} smfInfo;

extern const int32_t midiNoteFreq[];
extern const char midiNoteName[][5];


extern timebaseI24Q8_t smfLibUsToTimebase(int32_t timeUs, int32_t timeDiv, int32_t tempo);
extern uint32_t smfLibGetSmfFixedLe7(const uint8_t smfBuf[], off_t off, len_t len);
extern uint32_t smfLibGetSmfFixedBe8(const uint8_t smfBuf[], off_t off, len_t len);
extern len_t smfLibGetSmfVar(const uint8_t smfBuf[], off_t off, int32_t *value);
extern void smfLibRewindToStart(smfInfo *smfi);
extern int32_t smfLibInterpreterInit(smfInfo *smfi, const uint8_t buf[], int32_t bufLen);

#endif /* SMF_LIB_H_ */
