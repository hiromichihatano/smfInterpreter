/*
 * smf_trackHeader.c
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */


#include "smf_base.h"
#include "smf_lib.h"
#include "smf_debug.h"
#include "smf_trackHeader.h"

static smfoff_t _checkSmfTrackChunkType(const uint8_t smfBuf[], smfoff_t off)
{

	if(smfBuf[off + 0] == (uint8_t)'M' &&
		smfBuf[off + 1] == (uint8_t)'T' &&
		smfBuf[off + 2] == (uint8_t)'r' &&
		smfBuf[off + 3] == (uint8_t)'k'){
		return 4;
	}

	smfDbgPrintLog(SMFLOG_ERR, "Track Chunk is not matchked. @offset 0x%08x\n", off);
	return -1;
}

static smfoff_t _getSmfTrackBodyLength(const uint8_t smfBuf[], smfoff_t off, smfsize_t *bodylen)
{
	const smflen_t fixedSize = 4;
	smfsize_t retVal;

	retVal = (smfsize_t)smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	*bodylen = retVal;
	return fixedSize;
}

static smfoff_t _checkSmfTrackEnd(const uint8_t smfBuf[], smfoff_t off)
{
	if(smfBuf[off + 0] == 0xff &&
		smfBuf[off + 1] == 0x2f &&
		smfBuf[off + 2] == 0x00){
		return 3;
	}
	smfDbgPrintLog(SMFLOG_ERR, "Track End MetaEvent is not matchked. @offset 0x%08x\n", off);
	return -1;

}

smfoff_t smfInterpreterTrackInit(smfTrackInfo *tracki, const uint8_t buf[], smfoff_t beginOff)
{
	smfoff_t offset = beginOff, bodyOffset;
	smflen_t len;
	smfsize_t bodylen;
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

	smfDbgPrintLog(SMFLOG_INFO, "TrackInit OK. track Offset 0x%06x, Length %d, 1stEventTimeBase %d \n",
			beginOff, bodylen, nextEventTime);


	tracki->bodyOffset = bodyOffset;
	tracki->endOffset = bodyOffset + bodylen;
	tracki->nextEventOffset = bodyOffset;
	tracki->nextEventTime = nextEventTime;

	return offset;

ERROR:
	smfDbgPrintLog(SMFLOG_ERR, "SMF Track init Failed.\n");
	return -1;
}
