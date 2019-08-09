/*
 * smf_midiEvent.h
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#ifndef SMF_MIDIEVENT_H_
#define SMF_MIDIEVENT_H_

#include "smf.h"

typedef enum metaEventType {
	metaEventTypeSequenceNumber = 0x00,
	metaEventTypeText           = 0x01,
	metaEventTypeCopyright      = 0x02,
	metaEventTypeTitle          = 0x03,
	metaEventTypeInstrument     = 0x04,
	metaEventTypeLyrics         = 0x05,
	metaEventTypeMarker         = 0x06,
	metaEventTypeCueuePoint     = 0x07,
	metaEventTypeProgramName    = 0x08,
	metaEventTypeDevice         = 0x09,
	metaEventTypeChannelPrefix  = 0x20,
	metaEventTypePort           = 0x21,
	metaEventTypeTrackEnd       = 0x2f,
	metaEventTypeTempo          = 0x51,
	metaEventTypeSmpteOffset    = 0x54,
	metaEventTypeRhythm         = 0x58,		/* 拍子 */
	metaEventTypeKeySign        = 0x59,		/* 調号 */
} enumMetaEventType;



extern int32_t smfMidiEventTrackTimerTick(smfInfo *smfi, int32_t trackno, timebase_t time);
extern int32_t smfMidiEventTimerTick(smfInfo *smfi, timebase_t time);

#endif /* SMF_MIDIEVENT_H_ */
