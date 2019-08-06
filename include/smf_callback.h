/*
 * smf_callback.h
 *
 *  Created on: 2016/10/26
 *      Author: hiro
 */

#ifndef SMF_CALLBACK_H_
#define SMF_CALLBACK_H_

#include "smf.h"

int32_t smfCallbackNoteOff(uint8_t channel, uint8_t key, uint8_t velocity);
int32_t smfCallbackNoteOn(uint8_t channel, uint8_t key, uint8_t velocity);
int32_t smfCallbackPolyKeyPressure(uint8_t channel, uint8_t key, uint8_t velocity);
int32_t smfCallbackControlChange(uint8_t channel, uint8_t ctrlNum, uint8_t value);
int32_t smfCallbackProgramChange(uint8_t channel, uint8_t programNum);
int32_t smfCallbackChannelPressure(uint8_t channel, uint8_t pressureValue);
int32_t smfCallbackPitchBend(uint8_t channel, int32_t bendValue);
int32_t smfCallbackSysEx(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType);
int32_t smfCallbackMetaEvent(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType);
int32_t smfCallbackMetaEventTempo(int32_t tempo);

#endif /* SMF_CALLBACK_H_ */
