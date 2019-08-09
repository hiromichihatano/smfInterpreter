/*
 * smf_dummyCallback.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */


#include "smf.h"

#define SMF_PRINTF


__attribute__((weak))
int32_t smfCallbackNoteOff(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	SMF_PRINTF("NoteOFF ");
	SMF_PRINTF("Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackNoteOn(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	SMF_PRINTF("NoteON ");
	SMF_PRINTF("Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackPolyKeyPressure(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	SMF_PRINTF("PolyKeyPress ");
	SMF_PRINTF("Ch %d, Key %d, Vel %d\n", channel, key, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackControlChange(uint8_t channel, uint8_t ctrlNum, uint8_t value)
{
	UNUSED(channel); UNUSED(ctrlNum); UNUSED(value);
	SMF_PRINTF("CTRL ");
	SMF_PRINTF("Ch %d, ctrl %d, val %d\n", channel, ctrlNum, value);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackProgramChange(uint8_t channel, uint8_t programNum)
{
	UNUSED(channel); UNUSED(programNum);
	SMF_PRINTF("Prog ");
	SMF_PRINTF("Ch %d, prog %d\n", channel, programNum);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackChannelPressure(uint8_t channel, uint8_t pressureValue)
{
	UNUSED(channel); UNUSED(pressureValue);
	SMF_PRINTF("ChPress ");
	SMF_PRINTF("Ch %d, Pres %d\n", channel, pressureValue);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackPitchBend(uint8_t channel, int32_t bendValue)
{
	UNUSED(channel); UNUSED(bendValue);
	SMF_PRINTF("PBend ");
	SMF_PRINTF("Ch %d, Pbend %d\n", channel, bendValue);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackSysEx(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType)
{
	UNUSED(buf); UNUSED(beginOff); UNUSED(sysExLen); UNUSED(sysExType);
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	SMF_PRINTF("SysEx ");
	SMF_PRINTF("type: %x, len=%d, data:", sysExType, sysExLen);

	for(i=0; i<sysExLen; i++) {
		SMF_PRINTF("%02x ", buf[offset + i]);
	}
	SMF_PRINTF("\n");

	return 0;
}


__attribute__((weak))
int32_t smfCallbackMetaEvent(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType)
{
	UNUSED(buf); UNUSED(beginOff); UNUSED(metaEventLen); UNUSED(metaEventType);
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	SMF_PRINTF("type: %x, len=%d, data:", metaEventType, metaEventLen);

	for(i=0; i<metaEventLen; i++) {
		SMF_PRINTF("%02x ", buf[offset + i]);
	}
	SMF_PRINTF("\n");
	return 0;
}


__attribute__((weak))
int32_t smfCallbackMetaEventTempo(int32_t tempo)
{
	int32_t realTempo = 60 * 1000000 / tempo; UNUSED(realTempo);
	SMF_PRINTF("smfTempo %d (Real:%d)\n", tempo, realTempo);
	return 0;
}
