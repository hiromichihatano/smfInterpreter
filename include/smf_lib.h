/*
 * smf_lib.h
 *
 *  Created on: 2016/10/24
 *      Author: hiromichihatano
 */

#ifndef SMF_LIB_H_
#define SMF_LIB_H_

#include "smf_base.h"
#include "smf_callback.h"

extern const int32_t midiNoteFreq[];
extern const char midiNoteName[][5];


extern timebaseI24Q8_t smfLibUsToTimebase(int32_t timeUs, int32_t timeDiv, int32_t tempo);
extern uint32_t smfLibGetSmfFixedLe7(const uint8_t smfBuf[], smfoff_t off, smflen_t len);
extern uint32_t smfLibGetSmfFixedBe8(const uint8_t smfBuf[], smfoff_t off, smflen_t len);
extern smflen_t smfLibGetSmfVar(const uint8_t smfBuf[], smfoff_t off, int32_t *value);
extern void smfLibRewindToStart(smfInfo *smfi);
extern int32_t smfLibInterpreterInit(smfInfo *smfi, const uint8_t buf[], int32_t bufLen);
extern int32_t smfLibTimerTick(smfInfo *smfi, const smf_callback_t *smfcb, timebase_t time);
extern void smfLibRegisterLogFunc(smf_loglevel_t loglevel, print_log_t func);

#endif /* SMF_LIB_H_ */