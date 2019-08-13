/*
 * smf_trackHeader.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */


#include "smf.h"

#define SMF_PRINTF
#define SMF_EPRINTF

static off_t _checkSmfTrackChunkType(const uint8_t smfBuf[], off_t off)
{

	if(smfBuf[off + 0] == (uint8_t)'M' &&
		smfBuf[off + 1] == (uint8_t)'T' &&
		smfBuf[off + 2] == (uint8_t)'r' &&
		smfBuf[off + 3] == (uint8_t)'k'){
		return 4;
	}

	SMF_EPRINTF("Track Chunk is not matchked. @offset 0x%08x\n", off);
	return -1;
}

static off_t _getSmfTrackBodyLength(const uint8_t smfBuf[], off_t off, size_t *bodylen)
{
	const len_t fixedSize = 4;
	size_t retVal;

	retVal = (size_t)smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	*bodylen = retVal;
	return fixedSize;
}

static off_t _checkSmfTrackEnd(const uint8_t smfBuf[], off_t off)
{
	if(smfBuf[off + 0] == 0xff &&
		smfBuf[off + 1] == 0x2f &&
		smfBuf[off + 2] == 0x00){
		return 3;
	}
	SMF_EPRINTF("Track End MetaEvent is not matchked. @offset 0x%08x\n", off);
	return -1;

}

off_t smfInterpreterTrackInit(smfTrackInfo *tracki, const uint8_t buf[], off_t beginOff)
{
	off_t offset = beginOff, bodyOffset;
	len_t len;
	size_t bodylen;
	timebase_t nextEventTime;

	/* MThd の確認 */
	len = _checkSmfTrackChunkType(buf, offset);
	if (len < 0) goto ERROR;

	/* トラック長の取得 */
	offset += len;
	len = _getSmfTrackBodyLength(buf, offset, &bodylen);
	if (len < 0) goto ERROR;

	bodyOffset = offset + len;

	/* トラック末尾の確認 */
	offset += len + bodylen;
	len = _checkSmfTrackEnd(buf, offset - 3);
	if (len < 0) goto ERROR;

	/* 先頭イベント時間の取得 */
	len = smfLibGetSmfVar(buf, bodyOffset, &nextEventTime);
	if (len < 0) goto ERROR;

	SMF_PRINTF("TrackInit OK. track Offset 0x%06x, Length %d, "
			"1stEventTimeBase %d \n",
			beginOff, bodylen, nextEventTime);


	tracki->bodyOffset = bodyOffset;
	tracki->endOffset = bodyOffset + bodylen;
	tracki->nextEventOffset = bodyOffset;
	tracki->nextEventTime = nextEventTime;

	return offset;

ERROR:
	SMF_EPRINTF("SMF Track init Failed.\n");
	return -1;
}
