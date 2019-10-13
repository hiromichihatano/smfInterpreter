/*
 * smf_dummyCallback.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#include <stdio.h>
#include "smf.h"

static void _smfCallbackNoteOff(uint8_t channel, uint8_t key, uint8_t velocity)
{
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	printf("NoteOFF ");
	printf("Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
}


static void _smfCallbackNoteOn(uint8_t channel, uint8_t key, uint8_t velocity)
{
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	printf("NoteON ");
	printf("Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
}


static void _smfCallbackPolyKeyPressure(uint8_t channel, uint8_t key, uint8_t velocity)
{
	printf("PolyKeyPress ");
	printf("Ch %d, Key %d, Vel %d\n", channel, key, velocity);
}


static void _smfCallbackControlChange(uint8_t channel, uint8_t ctrlNum, uint8_t value)
{
	printf("CTRL ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, ctrl %d, val %d\n", channel, ctrlNum, value);
}


static void _smfCallbackProgramChange(uint8_t channel, uint8_t programNum)
{
	printf("Prog ");
	printf("Ch %d, prog %d\n", channel, programNum);
}


static void _smfCallbackChannelPressure(uint8_t channel, uint8_t pressureValue)
{
	printf("ChPress ");
	printf("Ch %d, Pres %d\n", channel, pressureValue);
}


static void _smfCallbackPitchBend(uint8_t channel, int32_t bendValue)
{
	printf("PBend ");
	printf("Ch %d, Pbend %d\n", channel, bendValue);
}


static void _smfCallbackSysEx(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType)
{
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	printf("SysEx ");
	printf("type: %x, len=%d, data:", sysExType, sysExLen);

	for(i=0; i<sysExLen; i++) {
		printf("%02x ", buf[offset + i]);
	}
	printf("\n");
}


static void _smfCallbackMetaEvent(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType)
{
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	printf("type: %x, len=%d, data:", metaEventType, metaEventLen);

	for(i=0; i<metaEventLen; i++) {
		printf("%02x ", buf[offset + i]);
	}
	printf("\n");
}


static void _smfCallbackMetaEventTempo(int32_t tempo)
{
	int32_t realTempo = 60 * 1000000 / tempo; UNUSED(realTempo);
	printf("smfTempo %d (Real:%d)\n", tempo, realTempo);
}


/**
 * @brief smf_callback 構造体に中身をセットする
 * 
 * @param smfcb 書き換え対象の構造体
 */
void smfsample_set_callbacks(smf_callback_t *smfcbp){
	smfcbp->noteOff = _smfCallbackNoteOff;
	smfcbp->noteOn = _smfCallbackNoteOn;
	smfcbp->polyKeyPressure = _smfCallbackPolyKeyPressure;
	smfcbp->controlChange = _smfCallbackControlChange;
	smfcbp->programChange = _smfCallbackProgramChange;
	smfcbp->channelPressure = _smfCallbackChannelPressure;
	smfcbp->pitchBend = _smfCallbackPitchBend;
	smfcbp->sysEx = _smfCallbackSysEx;
	smfcbp->metaEvent = _smfCallbackMetaEvent;
	smfcbp->metaEventTempo = _smfCallbackMetaEventTempo;
}

