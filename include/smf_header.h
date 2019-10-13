/*
 * smf_header.h
 *
 *  Created on: 2016/10/24
 *      Author: hiromichihatano
 */

#ifndef SMF_HEADER_H_
#define SMF_HEADER_H_

#include "smf_base.h"

extern smfoff_t smfInterpreterHeaderInit(smfInfo *smfi, const uint8_t buf[], smfoff_t beginOff);

#endif /* SMF_HEADER_H_ */
