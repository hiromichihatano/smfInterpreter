/*
 * smf_midiEvent.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#include "smf.h"

#define SMF_PRINTF
#define SMF_EPRINTF

static len_t _doEventNoteOff(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	ret = smfCallbackNoteOff((uint8_t)channel, (uint8_t)key, (uint8_t)velocity);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static len_t _doEventNoteOn(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 2;
	int32_t ret;
	const uint8_t noteOffStatusByte = 0x80;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	/** NoteOff の場合 */
	if(velocity == 0x00) {
		len = _doEventNoteOff(smfi, trackno, offset, (noteOffStatusByte | channel) );
	} else {
		ret = smfCallbackNoteOn(channel, key, velocity);
		if(ret < 0) {
			len = -1;
		}
	}
	return len;
}


static len_t _doEventPolyKeyPressure(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t key = buf[offset];
	uint8_t velocity = buf[offset + 1];

	ret = smfCallbackPolyKeyPressure(channel, key, velocity);
	if(ret < 0) {
		len = -1;
	}
	return len;
}


/**
 *
 * @note チャネルモードメッセージ非対応
 */
static len_t _doEventControlChange(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t ctrlNum = buf[offset];
	uint8_t value = buf[offset + 1];

	ret = smfCallbackControlChange(channel, ctrlNum, value);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static len_t _doEventProgramChange(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 1;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t programNum = buf[offset];


	ret = smfCallbackProgramChange(channel, programNum);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static len_t _doEventChannelPressure(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 1;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	uint8_t pressureValue = buf[offset];


	ret = smfCallbackChannelPressure(channel, pressureValue);
	if(ret < 0) {
		len = -1;
	}
	return len;
}


static len_t _doEventPitchBend(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len = 2;
	int32_t ret;

	uint8_t channel = statusByte & 0x0f;
	int32_t bendValue = (int32_t)smfLibGetSmfFixedLe7(buf, offset, 2) - 0x4000;

	ret = smfCallbackPitchBend(channel, bendValue);
	if(ret < 0) {
		len = -1;
	}

	return len;
}


static len_t _doEventSysEx(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	int32_t ret;

	uint8_t sysExType = statusByte;
	len_t sysExLen;
	len_t len = smfLibGetSmfVar(buf, offset, &sysExLen);
	if(len < 0) goto ERR;

	offset += len;
	ret = smfCallbackSysEx(buf, offset, sysExLen, sysExType);
	if(ret < 0) goto ERR;

	return len + sysExLen;

ERR:
	return -1;
}

static int32_t _switchMetaEvent(smfInfo *smfi, int32_t trackno, off_t beginOff, len_t metaEventLen, uint8_t metaEventType)
{
	const uint8_t *buf = smfi->smfDataBuf;
	int32_t ret;

	SMF_PRINTF("Meta ");
	switch((enumMetaEventType)metaEventType) {
	case metaEventTypeSequenceNumber:
		SMF_PRINTF("SequenceNumber ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeText:
		SMF_PRINTF("Text ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTitle:
		SMF_PRINTF("Title ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeInstrument:
		SMF_PRINTF("Inst ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeProgramName:
		SMF_PRINTF("ProgName ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeDevice:
		SMF_PRINTF("Device ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeChannelPrefix:
		SMF_PRINTF("ChPref. ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypePort:
		SMF_PRINTF("Port ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTrackEnd:
		SMF_PRINTF("TrackEnd ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeTempo:
		smfi->smfTempo = smfLibGetSmfFixedBe8(buf, beginOff, metaEventLen);
		ret = smfCallbackMetaEventTempo(smfi->smfTempo);
		break;

	case metaEventTypeSmpteOffset:
		SMF_PRINTF("SMPTE ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeRhythm:
		SMF_PRINTF("Rhythm ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	case metaEventTypeKeySign:
		SMF_PRINTF("KeySign ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;

	default:
		SMF_PRINTF("?? ");
		ret = smfCallbackMetaEvent(buf, beginOff, metaEventLen, metaEventType);
		break;
	}

	return ret;
}

static len_t _doEventMetaEvent(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t statusByte) {
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	int32_t ret;

	uint8_t metaEventType = buf[offset];
	offset++;

	len_t metaEventLen;
	len_t len = smfLibGetSmfVar(buf, offset, &metaEventLen);
	if(len < 0) goto ERR;

	offset += len;
	ret = _switchMetaEvent(smfi, trackno, offset, metaEventLen, metaEventType);
	if(ret < 0) goto ERR;

	return len + 1 + metaEventLen;

ERR:
	SMF_EPRINTF("_doEventMetaEvent, offset %d (0x%x)\n", offset, offset);
	return -1;
}


static len_t _doEvent(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t *prevStatusByte)
{
	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len1=0, len2=0;
	uint8_t statusByte = buf[beginOffset];

	if(statusByte<0x80) { 	// ランニングステータスの場合
		statusByte = *prevStatusByte;
	} else {
		offset += 1;		// statusByte 分を進める
		len1 += 1;
	}

	if(statusByte < 0x80) {
		SMF_EPRINTF("previous StatusByte 0x%02x is Invalid.\n", statusByte);
		goto ERR;
	} else if (statusByte <= 0x8f) {
		len2 = _doEventNoteOff(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0x9f) {
		len2 = _doEventNoteOn(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0xaf) {
		len2 = _doEventPolyKeyPressure(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0xbf) {
		len2 = _doEventControlChange(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0xcf) {
		len2 = _doEventProgramChange(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0xdf) {
		len2 = _doEventChannelPressure(smfi, trackno, offset, statusByte);
	} else if (statusByte <= 0xef) {
		len2 = _doEventPitchBend(smfi, trackno, offset, statusByte);
	} else if (statusByte == 0xf0 || statusByte == 0xf7) {
		len2 = _doEventSysEx(smfi, trackno, offset, statusByte);
	} else if (statusByte == 0xff) {
		len2 = _doEventMetaEvent(smfi, trackno, offset, statusByte);
	} else {
		SMF_EPRINTF("StatusByte 0x%02x is Invalid.\n", statusByte);
		goto ERR;
	}

	*prevStatusByte = statusByte;
	return len1 + len2;		/* statusByte(1byte) + statusBody(*) */

ERR:
	return -1;
}

len_t smfMidiEventParseOneEvent(smfInfo *smfi, int32_t trackno, off_t beginOffset, uint8_t *prevStatusByte){

	const uint8_t *buf = smfi->smfDataBuf;
	off_t offset = beginOffset;
	len_t len1, len2;

	len1 = smfLibGetSmfVar(buf, offset, NULL);
	if(len1 < 0) goto ERR;

	offset += len1;
	len2 = _doEvent(smfi, trackno, offset, prevStatusByte);
	if(len2 < 0) goto ERR;

	return len1 + len2;		/* deltaTimeLen + MidiEventLen */


ERR:
	SMF_EPRINTF("getEvent offset %d, (0x%x)\n", offset, offset);
	return -1;

}


int32_t smfMidiEventTrackTimerTick(smfInfo *smfi, int32_t trackno, timebase_t time)
{
	const uint8_t *buf = smfi->smfDataBuf;
	smfTrackInfo *tracki = &(smfi->tracki[trackno]);
	timebase_t nextEventTime = tracki->nextEventTime;
	uint8_t prevStateByte = tracki->prevStateByte;
	off_t offset = tracki->nextEventOffset;
	len_t len;

	if (tracki->endOffset <= offset) {
		return 1;	/* トラックの最後 */
	}

	while(nextEventTime < time) {
		timebase_t nextDelta;
		SMF_PRINTF("[%4d/%3d] (%6d, +0x%08x) <Ch %02d> ",
				nextEventTime / smfi->smfTimeDivision,
				nextEventTime % smfi->smfTimeDivision,
				nextEventTime,
				offset,
				trackno);
		len = smfMidiEventParseOneEvent(smfi, trackno, offset, &prevStateByte);
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
	SMF_EPRINTF("on parsing, offset %d (0x%x)\n", offset, offset);
	return -1;
}

int32_t smfMidiEventTimerTick(smfInfo *smfi, timebase_t time)
{
	int32_t endTracks = 0;
	for(int32_t i=0; i<smfi->smfNrTracks; i++) {
		int32_t ret = smfMidiEventTrackTimerTick(smfi, i, time);
		if(ret < 0) return -1;
		endTracks += ret;
	}
	if(smfi->smfNrTracks <= endTracks) {
		return 1;
	}
	return 0;
}
