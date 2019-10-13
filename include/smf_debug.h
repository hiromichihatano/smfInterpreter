/*
 * smf_debug.h
 *
 *  Created on: 2019/08/16
 *      Author: hiromichihatano
 */
#ifndef SMF_DEBUG_H_
#define SMF_DEBUG_H_

#include <stdarg.h>
#include "smf_base.h"

extern void smfDbgRegisterLogFunc(smf_loglevel_t loglevel, print_log_t func);
extern void smfDbgPrintLog(smf_loglevel_t loglevel, const char *format, ...);


#endif