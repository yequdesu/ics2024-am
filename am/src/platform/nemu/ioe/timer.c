#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}
// nemu.h: RTC_ADDR        (DEVICE_BASE + 0x0000048)
void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = 0;
  uint32_t us_h = inl(RTC_ADDR + 4);
  uint32_t us_l = inl(RTC_ADDR);
  uint64_t us = ((uint64_t)us_h << 32) | (uint64_t)us_l;
  uptime->us = us;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
