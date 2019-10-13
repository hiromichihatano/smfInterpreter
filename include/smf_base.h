/*
 * smf_base.h
 *
 *  Created on: 2019/10/13
 *      Author: hiromichihatano
 */
#ifndef SMF_BASE_H_
#define SMF_BASE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define MAXTRACKS (20)

#ifndef UNUSED
	#define UNUSED(x) ((void)(x))
#endif

typedef int32_t smfsize_t;
typedef int32_t smfoff_t;
typedef int32_t smflen_t;
typedef int32_t timebase_t;
typedef int32_t timebaseI24Q8_t;

typedef int (*print_log_t)(const char *format, va_list ap);

typedef struct {
	smfoff_t bodyOffset;		/** MIDIデータ本体のオフセット */
	smflen_t endOffset;			/** MIDI body の長さ */
	smfoff_t nextEventOffset;	/** 次に読むMIDIイベントの先頭を指す */
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


typedef enum smf_loglevel {
    SMFLOG_INFO = 0,
    SMFLOG_STD,
    SMFLOG_ERR,
    SMFLOG_MAX,
} smf_loglevel_t;

#endif /* SMF_BASE_H_ */