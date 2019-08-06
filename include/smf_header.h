/*
 * smf_initHeader.h
 *
 *  Created on: 2016/10/24
 *      Author: hiro
 */

#ifndef SMF_HEADER_H_
#define SMF_HEADER_H_

#include "smf.h"


off_t smfInterpreterHeaderInit(smfInfo *smfi, const uint8_t buf[], off_t beginOff);


#endif /* SMF_HEADER_H_ */
