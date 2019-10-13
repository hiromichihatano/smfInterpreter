/*
 * smf_trackHeader.h
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#ifndef SMF_TRACKHEADER_H_
#define SMF_TRACKHEADER_H_

#include "smf_base.h"

extern smfoff_t smfInterpreterTrackInit(smfTrackInfo *tracki, const uint8_t buf[], smfoff_t beginOff);

#endif /* SMF_TRACKHEADER_H_ */
