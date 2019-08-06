/*
 * smf_lib.h
 *
 *  Created on: 2016/10/24
 *      Author: hiro
 */

#ifndef SMF_LIB_H_
#define SMF_LIB_H_

//typedef int32_t size_t;
typedef int32_t off_t;
typedef int32_t len_t;
typedef int32_t timebase_t;
typedef int32_t timebaseI24Q8_t;

#define MAXTRACKS (20)

#ifndef UNUSED
	#define UNUSED(x) ((void)(x))
#endif

typedef struct {
	off_t bodyOffset;			/** MIDI�f�[�^�{�̂̃I�t�Z�b�g */
	len_t endOffset;			/** MIDI body �̒��� */
	off_t nextEventOffset;		/** ���ɓǂ�MIDI�C�x���g�̐擪���w�� */
	timebase_t nextEventTime;	/** ���̃C�x���g����(�ώZ����) */
	uint8_t prevStateByte;		/** �O�̃C�x���g�� statusByte */

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

extern const int32_t midiNoteFreq[];
extern const int8_t midiNoteName[][5];


timebaseI24Q8_t smfLibUsToTimebase(int32_t timeUs, int32_t timeDiv, int32_t tempo);
uint32_t smfLibGetSmfFixedLe7(const uint8_t smfBuf[], off_t off, len_t len);
uint32_t smfLibGetSmfFixedBe8(const uint8_t smfBuf[], off_t off, len_t len);
len_t smfLibGetSmfVar(const uint8_t smfBuf[], off_t off, int32_t *value);
void showBinary(uint8_t buf[], int32_t len);
void smfLibRewindToStart(smfInfo *smfi);
int32_t smfLibInterpreterInit(smfInfo *smfi, const uint8_t buf[], int32_t bufLen);

#endif /* SMF_LIB_H_ */
