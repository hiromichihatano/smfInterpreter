/*
 * smf_trackHeader.h
 *
 *  Created on: 2016/10/26
 *      Author: hiro
 */

#ifndef SMF_TRACKHEADER_H_
#define SMF_TRACKHEADER_H_


off_t smfInterpreterTrackInit(smfTrackInfo *tracki, const uint8_t buf[], off_t beginOff);

#endif /* SMF_TRACKHEADER_H_ */
