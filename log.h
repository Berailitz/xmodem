#ifndef MEMPOOL_LOG_H
#define MEMPOOL_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* 日志级别 */
typedef enum {
    LTRACE, LDEBUG, LINFO, LERROR
} LOG_LEVEL;

/* 日志转换函数 */
#ifdef __MINGW32__
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif
#define __FILENAME__ (strrchr(__FILE__, DIR_SEP) ? strrchr(__FILE__, DIR_SEP) + 1 : __FILE__)
#define LOG_ARGS __FILENAME__, __LINE__, __func__
#define SLOG_ARGS LOG_ARGS, self

/* 换行符 */
const char *const ENDL;

/* 日志函数 */
#define slog(log_level, args...) log_base(log_level, SLOG_ARGS, "", args)

/* 分割行 */
const char * const LINE_SEP_TPL;
#define slog_sep(log_level, text) log_base(log_level, SLOG_ARGS, ENDL, LINE_SEP_TPL, text)
#define plog_sep(log_level, text) log_base(log_level, LOG_ARGS, NULL, ENDL, LINE_SEP_TPL, text)

/* 不换行的日志头尾 */
#define log_head(level) slog(level, "")
#define log_tail() printf(ENDL)

/* 不同级别的日志入口函数 */
#define strace(args...) log_base(LTRACE, SLOG_ARGS, ENDL, args)
#define sdebug(args...) log_base(LDEBUG, SLOG_ARGS, ENDL, args)
#define sinfo(args...)  log_base(LINFO,  SLOG_ARGS, ENDL, args)
#define serror(args...) log_base(LERROR, SLOG_ARGS, ENDL, args)
#define ptrace(args...) log_base(LTRACE, LOG_ARGS, NULL, ENDL, args)
#define pdebug(args...) log_base(LDEBUG, LOG_ARGS, NULL, ENDL, args)
#define pinfo(args...)  log_base(LINFO,  LOG_ARGS, NULL, ENDL, args)
#define perror(args...) log_base(LERROR, LOG_ARGS, NULL, ENDL, args)

#define DE_SELF void *self = NULL;

/* 日志输出函数 */
void log_base(const LOG_LEVEL level, const char *const file, const int line, const char *const func,
              const void *const self, const char *const endl, const char *const fmt, ...);

#endif //MEMPOOL_LOG_H
