/*
 * smf_callback.h
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#ifndef SMF_CALLBACK_H_
#define SMF_CALLBACK_H_

#include "smf.h"

extern int32_t smfCallbackNoteOff(uint8_t channel, uint8_t key, uint8_t velocity);
extern int32_t smfCallbackNoteOn(uint8_t channel, uint8_t key, uint8_t velocity);
extern int32_t smfCallbackPolyKeyPressure(uint8_t channel, uint8_t key, uint8_t velocity);
extern int32_t smfCallbackControlChange(uint8_t channel, uint8_t ctrlNum, uint8_t value);
extern int32_t smfCallbackProgramChange(uint8_t channel, uint8_t programNum);
extern int32_t smfCallbackChannelPressure(uint8_t channel, uint8_t pressureValue);
extern int32_t smfCallbackPitchBend(uint8_t channel, int32_t bendValue);
extern int32_t smfCallbackSysEx(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType);
extern int32_t smfCallbackMetaEvent(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType);
extern int32_t smfCallbackMetaEventTempo(int32_t tempo);

#endif /* SMF_CALLBACK_H_ */
