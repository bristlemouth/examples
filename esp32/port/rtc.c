#include "bm_rtc.h"
#include <time.h>
#include <sys/time.h>

BmErr bm_rtc_set(const RtcTimeAndDate *time) {
    BmErr err = BmOK;

    struct tm tm = {0};
    time_t t;
    struct timeval tv = {0};

    tm.tm_year = time->year - 1900;
    tm.tm_mon = time->month - 1;
    tm.tm_mday = time->day;
    tm.tm_hour = time->hour;
    tm.tm_sec = time->second;
    t = mktime(&tm);
    tv.tv_sec = t;
    tv.tv_usec = time->ms * 1000;

    if (settimeofday(&tv, NULL) == -1) {
        err = BmEBADMSG;
    }

    return err;
}
BmErr bm_rtc_get(RtcTimeAndDate *time) {
    BmErr err = BmEBADMSG;
    struct timeval tv = {0};
    time_t t;
    struct tm *tm = NULL;

    if (gettimeofday(&tv, NULL) == 0) {
        err = BmOK;
        t = tv.tv_sec;
        tm = gmtime(&t);

        time->year = tm->tm_year + 1900;
        time->month = tm->tm_mon + 1;
        time->day = tm->tm_mday;
        time->hour = tm->tm_hour;
        time->minute = tm->tm_min;
        time->second = tm->tm_sec;
        time->ms = tv.tv_usec / 1000;
    }

    return err;
}

uint64_t bm_rtc_get_micro_seconds(RtcTimeAndDate *time_and_date) {
    uint64_t us = 0;
    struct timeval tv = {0};

    if (gettimeofday(&tv, NULL) == 0) {
        us = tv.tv_sec * 1000000 + tv.tv_usec;
    }

    return us;
}
