#include <stddef.h>
#include <smf_debug.h>
#include <stdarg.h>
#include "smf_base.h"
#include "smf_debug.h"

static print_log_t _printLogFuncs[SMFLOG_MAX] = {NULL, NULL, NULL};


/**
 * @brief ログ出力用の関数(vprintf 形式) を登録する
 * 
 * @param loglevel 登録する関数のログレベル
 * @param func 登録する関数。登録を解除する場合は、NULL を指定する。
 */
void smfDbgRegisterLogFunc(smf_loglevel_t loglevel, print_log_t func)
{
    if((smf_loglevel_t)0 <= loglevel && loglevel < SMFLOG_MAX) {
        _printLogFuncs[loglevel] = func;
    }
}

void smfDbgPrintLog(smf_loglevel_t loglevel, const char *format, ...)
{
    if((smf_loglevel_t)0 <= loglevel && loglevel < SMFLOG_MAX && _printLogFuncs[loglevel] != NULL) {
        va_list ap;
        va_start(ap, format);
        _printLogFuncs[loglevel](format, ap);
        va_end(ap);
    }
}

