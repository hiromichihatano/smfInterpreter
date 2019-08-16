/*
 * smf_dummyCallback.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */


#include "smf.h"


__attribute__((weak))
int32_t smfCallbackNoteOff(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	smfDbgPrintLog(SMFLOG_INFO, "NoteOFF ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackNoteOn(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	const char *keyName = midiNoteName[key]; UNUSED(keyName);
	smfDbgPrintLog(SMFLOG_INFO, "NoteON ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, Key %s, Vel %d\n", channel, keyName, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackPolyKeyPressure(uint8_t channel, uint8_t key, uint8_t velocity)
{
	UNUSED(channel); UNUSED(key); UNUSED(velocity);
	smfDbgPrintLog(SMFLOG_INFO, "PolyKeyPress ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, Key %d, Vel %d\n", channel, key, velocity);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackControlChange(uint8_t channel, uint8_t ctrlNum, uint8_t value)
{
	UNUSED(channel); UNUSED(ctrlNum); UNUSED(value);
	smfDbgPrintLog(SMFLOG_INFO, "CTRL ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, ctrl %d, val %d\n", channel, ctrlNum, value);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackProgramChange(uint8_t channel, uint8_t programNum)
{
	UNUSED(channel); UNUSED(programNum);
	smfDbgPrintLog(SMFLOG_INFO, "Prog ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, prog %d\n", channel, programNum);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackChannelPressure(uint8_t channel, uint8_t pressureValue)
{
	UNUSED(channel); UNUSED(pressureValue);
	smfDbgPrintLog(SMFLOG_INFO, "ChPress ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, Pres %d\n", channel, pressureValue);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackPitchBend(uint8_t channel, int32_t bendValue)
{
	UNUSED(channel); UNUSED(bendValue);
	smfDbgPrintLog(SMFLOG_INFO, "PBend ");
	smfDbgPrintLog(SMFLOG_INFO, "Ch %d, Pbend %d\n", channel, bendValue);
	return 0;
}


__attribute__((weak))
int32_t smfCallbackSysEx(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType)
{
	UNUSED(buf); UNUSED(beginOff); UNUSED(sysExLen); UNUSED(sysExType);
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	smfDbgPrintLog(SMFLOG_INFO, "SysEx ");
	smfDbgPrintLog(SMFLOG_INFO, "type: %x, len=%d, data:", sysExType, sysExLen);

	for(i=0; i<sysExLen; i++) {
		smfDbgPrintLog(SMFLOG_INFO, "%02x ", buf[offset + i]);
	}
	smfDbgPrintLog(SMFLOG_INFO, "\n");

	return 0;
}


__attribute__((weak))
int32_t smfCallbackMetaEvent(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType)
{
	UNUSED(buf); UNUSED(beginOff); UNUSED(metaEventLen); UNUSED(metaEventType);
	int32_t i;
	off_t offset = beginOff; UNUSED(offset);

	smfDbgPrintLog(SMFLOG_INFO, "type: %x, len=%d, data:", metaEventType, metaEventLen);

	for(i=0; i<metaEventLen; i++) {
		smfDbgPrintLog(SMFLOG_INFO, "%02x ", buf[offset + i]);
	}
	smfDbgPrintLog(SMFLOG_INFO, "\n");
	return 0;
}


__attribute__((weak))
int32_t smfCallbackMetaEventTempo(int32_t tempo)
{
	int32_t realTempo = 60 * 1000000 / tempo; UNUSED(realTempo);
	smfDbgPrintLog(SMFLOG_INFO, "smfTempo %d (Real:%d)\n", tempo, realTempo);
	return 0;
}
