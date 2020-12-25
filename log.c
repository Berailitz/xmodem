#include "log.h"

/* 级别对应的文字 */
const char level_strings[] = {
        'T', 'D', 'I', 'E'
};

/* 需要被输出的最小日志级别 */
LOG_LEVEL MIN_LOG_LEVEL = LINFO;

/* 换行符 */
const char *const ENDL = "\n";

/* 分割行 */
const char * const LINE_SEP_TPL = "==========%s==========";

/* 日志输出函数 */
void log_base(const LOG_LEVEL level, const char *const file, const int line, const char *const func,
              const void *const self, const char *const endl, const char *const fmt, ...) {
    if (level >= MIN_LOG_LEVEL) {
        va_list args;
        char buf[5];
        time_t t = time(NULL);
        buf[strftime(buf, sizeof(buf), "%M%S", localtime(&t))] = '\0';
        printf("%c%s %s:%d:%s %p ", level_strings[level], buf, file, line, func, self);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("%s", endl);
        fflush(stdout);
    }
}
