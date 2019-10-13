/*
 * smf_midiEvent.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#include "smf_base.h"
#include "smf_lib.h"
#include "smf_callback.h"
#include "smf_debug.h"
#include "smf_midiEvent.h"

static smflen_t _doEventNoteOff(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	smfcb->noteOff((uint8_t)channel, (uint8_t)key, (uint8_t)velocity);

	return len;
}


static smflen_t _doEventNoteOn(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 2;
	int32_t ret;
	const uint8_t noteOffStatusByte = 0x80;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	/** NoteOff の場合 */
	if(velocity == 0x00) {
		len = _doEventNoteOff(smfi, smfcb, trackno, offset, (noteOffStatusByte | channel) );
	} else {
		smfcb->noteOn(channel, key, velocity);
	}
	return len;
}


static smflen_t _doEventPolyKeyPressure(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	smfcb->polyKeyPressure(channel, key, velocity);
	return len;
}


/**
 *
 * @note チャネルモードメッセージ非対応
 */
static smflen_t _doEventControlChange(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t ctrlNum = buf[offset];
	uint8_t value = buf[offset + 1];

	smfcb->controlChange(channel, ctrlNum, value);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static smflen_t _doEventProgramChange(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 1;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t programNum = buf[offset];


	smfcb->programChange(channel, programNum);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static smflen_t _doEventChannelPressure(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 1;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t pressureValue = buf[offset];


	smfcb->channelPressure(channel, pressureValue);
	if(ret < 0) {
		len = -1;
	}
	return len;
}


static smflen_t _doEventPitchBend(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	int32_t bendValue = (int32_t)smfLibGetSmfFixedLe7(buf, offset, 2) - 0x4000;

	smfcb->pitchBend(channel, bendValue);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static smflen_t _doEventSysEx(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	int32_t ret;

	uint8_t sysExType = statusByte;
	smflen_t sysExLen;
	smflen_t len = smfLibGetSmfVar(buf, offset, &sysExLen);
	if(len < 0) goto ERR;

	offset += len;
	smfcb->sysEx(buf, offset, sysExLen, sysExType);
	if(ret < 0) goto ERR;

	return len + sysExLen;

ERR:
	return -1;
}

static int32_t _switchMetaEvent(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOff, smflen_t metaEventLen, uint8_t metaEventType)
{
	const uint8_t *buf = smfi->smfDataBuf;
	int32_t ret;

	smfDbgPrintLog(SMFLOG_INFO, "Meta ");
	switch((enumMetaEventType)metaEventType) {
	case metaEventTypeSequenceNumber:
		smfDbgPrintLog(SMFLOG_INFO, "SequenceNumber ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeText:
		smfDbgPrintLog(SMFLOG_INFO, "Text ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTitle:
		smfDbgPrintLog(SMFLOG_INFO, "Title ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeInstrument:
		smfDbgPrintLog(SMFLOG_INFO, "Inst ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeProgramName:
		smfDbgPrintLog(SMFLOG_INFO, "ProgName ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeDevice:
		smfDbgPrintLog(SMFLOG_INFO, "Device ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeChannelPrefix:
		smfDbgPrintLog(SMFLOG_INFO, "ChPref. ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypePort:
		smfDbgPrintLog(SMFLOG_INFO, "Port ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTrackEnd:
		smfDbgPrintLog(SMFLOG_INFO, "TrackEnd ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTempo:
		smfi->smfTempo = smfLibGetSmfFixedBe8(buf, beginOff, metaEventLen);
		smfcb->metaEventTempo(smfi->smfTempo);
		break;

	case metaEventTypeSmpteOffset:
		smfDbgPrintLog(SMFLOG_INFO, "SMPTE ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeRhythm:
		smfDbgPrintLog(SMFLOG_INFO, "Rhythm ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeKeySign:
		smfDbgPrintLog(SMFLOG_INFO, "KeySign ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	default:
		smfDbgPrintLog(SMFLOG_INFO, "?? ");
		smfcb->metaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;
	}

	return ret;
}

static smflen_t _doEventMetaEvent(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	int32_t ret;

	uint8_t metaEventType = buf[offset];
	offset++;

	smflen_t metaEventLen;
	smflen_t len = smfLibGetSmfVar(buf, offset, &metaEventLen);
	if(len < 0) goto ERR;

	offset += len;
	ret = _switchMetaEvent(smfi, smfcb, trackno, offset, metaEventLen, metaEventType);
	if(ret < 0) goto ERR;

	return len + 1 + metaEventLen;

ERR:
	smfDbgPrintLog(SMFLOG_ERR, "_doEventMetaEvent, offset %d (0x%x)\n", offset, offset);
	return -1;
}


static smflen_t _doEvent(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t *prevStatusByte)
{
	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len1=0, len2=0;
	uint8_t statusByte = buf[beginOffset];

	if(statusByte<0x80) { 	// ランニングステータスの場合
		statusByte = *prevStatusByte;
	} else {
		offset += 1;		// statusByte 分を進める
		len1 += 1;
	}

	if(statusByte < 0x80) {
		smfDbgPrintLog(SMFLOG_ERR, "previous StatusByte 0x%02x is Invalid.\n", statusByte);
		goto ERR;
	} else if (statusByte <= 0x8f) {
		len2 = _doEventNoteOff(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0x9f) {
		len2 = _doEventNoteOn(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0xaf) {
		len2 = _doEventPolyKeyPressure(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0xbf) {
		len2 = _doEventControlChange(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0xcf) {
		len2 = _doEventProgramChange(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0xdf) {
		len2 = _doEventChannelPressure(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte <= 0xef) {
		len2 = _doEventPitchBend(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte == 0xf0 || statusByte == 0xf7) {
		len2 = _doEventSysEx(smfi, smfcb, trackno, offset, statusByte);
	} else if (statusByte == 0xff) {
		len2 = _doEventMetaEvent(smfi, smfcb, trackno, offset, statusByte);
	} else {
		smfDbgPrintLog(SMFLOG_ERR, "StatusByte 0x%02x is Invalid.\n", statusByte);
		goto ERR;
	}

	*prevStatusByte = statusByte;
	return len1 + len2;		/* statusByte(1byte) + statusBody(*) */

ERR:
	return -1;
}

static smflen_t _smfMidiEventParseOneEvent(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, smfoff_t beginOffset, uint8_t *prevStatusByte){

	const uint8_t *buf = smfi->smfDataBuf;
	smfoff_t offset = beginOffset;
	smflen_t len1, len2;

	// Midi Event の長さを得る
	len1 = smfLibGetSmfVar(buf, offset, NULL);
	if(len1 < 0) goto ERR;

	offset += len1;
	len2 = _doEvent(smfi, smfcb, trackno, offset, prevStatusByte);
	if(len2 < 0) goto ERR;

	return len1 + len2;		/* deltaTimeLen + MidiEventLen */


ERR:
	smfDbgPrintLog(SMFLOG_ERR, "getEvent offset %d, (0x%x)\n", offset, offset);
	return -1;

}


static int32_t _trackTimerTick(smfInfo *smfi, const smf_callback_t *smfcb, int32_t trackno, timebase_t time)
{
	const uint8_t *buf = smfi->smfDataBuf;
	smfTrackInfo *tracki = &(smfi->tracki[trackno]);
	timebase_t nextEventTime = tracki->nextEventTime;
	uint8_t prevStateByte = tracki->prevStateByte;
	smfoff_t offset = tracki->nextEventOffset;
	smflen_t len;

	if (tracki->endOffset <= offset) {
		return 1;	/* トラックの最後 */
	}

	while(nextEventTime < time) {
		timebase_t nextDelta;
		smfDbgPrintLog(SMFLOG_INFO, "[%4d/%3d] (%6d, +0x%08x) <Ch %02d> ",
				nextEventTime / smfi->smfTimeDivision,
				nextEventTime % smfi->smfTimeDivision,
				nextEventTime,
				offset,
				trackno);
		len = _smfMidiEventParseOneEvent(smfi, smfcb, trackno, offset, &prevStateByte);
		if(len < 0) goto ERR;


		offset += len;

		smfLibGetSmfVar(buf, offset, &nextDelta);
		nextEventTime += nextDelta;
	}

	tracki->nextEventTime = nextEventTime;
	tracki->nextEventOffset = offset;
	tracki->prevStateByte = prevStateByte;

	return 0;

ERR:
	smfDbgPrintLog(SMFLOG_ERR, "on parsing, offset %d (0x%x)\n", offset, offset);
	return -1;
}

int32_t smfMidiEventTimerTick(smfInfo *smfi, const smf_callback_t *smfcb, timebase_t time)
{
	int32_t endTracks = 0;
	for(int32_t i=0; i<smfi->smfNrTracks; i++) {
		int32_t ret = _trackTimerTick(smfi, smfcb, i, time);
		if(ret < 0) return -1;
		endTracks += ret;
	}
	if(smfi->smfNrTracks <= endTracks) {
		return 1;
	}
	return 0;
}
