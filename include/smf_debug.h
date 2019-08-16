/*
 * smf_debug.h
 *
 *  Created on: 2019/08/16
 *      Author: hiromichihatano
 */
#ifndef SMF_DEBUG_H_
#define SMF_DEBUG_H_

#include <stdarg.h>

typedef enum smf_loglevel {
    SMFLOG_INFO = 0,
    SMFLOG_STD,
    SMFLOG_ERR,
    SMFLOG_MAX,
} smf_loglevel_t;


typedef int (*print_log_t)(const char *format, va_list ap);

extern void smfDbgRegisterLogFunc(smf_loglevel_t loglevel, print_log_t func);
extern void smfDbgPrintLog(smf_loglevel_t loglevel, const char *format, ...);


#endif