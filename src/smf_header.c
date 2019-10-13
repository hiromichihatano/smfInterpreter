/*
 * smf_initHeader.c
 *
 *  Created on: 2016/10/24
 *      Author: hiromichihatano
 */

#include "smf_base.h"
#include "smf_lib.h"
#include "smf_debug.h"
#include "smf_header.h"

static smflen_t _checkSmfHeaderChunkType(const uint8_t smfBuf[], smfoff_t off)
{

	if(smfBuf[off + 0] == (uint8_t)'M' &&
		smfBuf[off + 1] == (uint8_t)'T' &&
		smfBuf[off + 2] == (uint8_t)'h' &&
		smfBuf[off + 3] == (uint8_t)'d'){
		return 4;
	}

	return -1;
}


static smflen_t _getSmfHeaderLength(const uint8_t smfBuf[], smfoff_t off, int32_t *headerlen)
{
	const smflen_t fixedSize = 4;
	const int32_t expVal = 6;
	int32_t retVal;

	retVal = smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	if(retVal != expVal) {
		smfDbgPrintLog(SMFLOG_ERR, "SMF Header Length is %d. (expected 6)\n");
		return -1;
	}

	*headerlen = retVal;
	return fixedSize;
}


static smflen_t _getSmfFormatType(const uint8_t smfBuf[], smfoff_t off, int32_t *format)
{
	const smflen_t fixedSize = 2;
	int32_t retVal;

	retVal = smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	if(retVal < 0 || 2 < retVal) {
		smfDbgPrintLog(SMFLOG_ERR, "SMF Format Type is %d. (expected between 0 and 2)\n");
		return -1;
	}

	*format = retVal;
	return fixedSize;

}

static smflen_t _getSmfNRTracks(const uint8_t smfBuf[], smfoff_t off, int32_t *tracks)
{
	const smflen_t fixedSize = 2;
	int32_t retVal;

	retVal = smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	if(MAXTRACKS < retVal) {
		smfDbgPrintLog(SMFLOG_ERR, "SMF Number of Tracks is %d. (OVER %d)\n", retVal, MAXTRACKS);
		return -1;
	}

	*tracks = retVal;
	return fixedSize;

}


static smflen_t _getSmfTimeDivision(const uint8_t smfBuf[], smfoff_t off, int32_t *timeBase)
{
	const smflen_t fixedSize = 2;
	int32_t retVal;

	retVal = smfLibGetSmfFixedBe8(smfBuf, off, fixedSize);

	if(0x8000 < retVal) {
		smfDbgPrintLog(SMFLOG_ERR, "SMF Time Base is %d. (expected lower than 16)\n");
		return -1;
	}

	*timeBase = retVal;
	return fixedSize;
}

smfoff_t smfInterpreterHeaderInit(smfInfo *smfi, const uint8_t buf[], smfoff_t beginOff)
{
	smfoff_t offset = beginOff;
	smflen_t len;
	int32_t headerLen = -1, format = -1, nrTracks=-1, timeDivision=-1;

	len = _checkSmfHeaderChunkType(buf, offset);
	if(len < 0) goto ERROR;

	offset += len;
	len = _getSmfHeaderLength(buf, offset, &headerLen);
	if(len < 0) goto ERROR;

	offset += len;
	len = _getSmfFormatType(buf, offset, &format);
	if(len < 0) goto ERROR;

	offset += len;
	len = _getSmfNRTracks(buf, offset, &nrTracks);
	if(len < 0) goto ERROR;

	offset += len;
	len = _getSmfTimeDivision(buf, offset, &timeDivision);
	if(len < 0) goto ERROR;

	smfi->smfFormat = format;
	smfi->smfNrTracks = nrTracks;
	smfi->smfTimeDivision = timeDivision;
	smfi->smfTempo = 1000 * 1000; 	/* 初期値は 60 にしておく */

	smfDbgPrintLog(SMFLOG_STD, "Header Init OK. HdLEN:%d Format:%d, Tracks:%d, TimeDiv:%d\n",
			headerLen, format, nrTracks, timeDivision);

	return offset + len;

ERROR:
	smfDbgPrintLog(SMFLOG_ERR, "SMF Init Failed.\n");
	return -1;
}

