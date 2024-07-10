#ifndef global_h__
#define global_h__

#include "api_mgr.h"
#include "gpio.h"
#include "inifile.h"
#include "net_timer.h"
#include "pwm.h"
#include "rs485.h"
#include "sys_timer.h"
#include "thread_group.h"


namespace net_code
{
const uint32_t active = 0x000001;
const uint32_t active_resp = 0x000002;

const uint32_t login = 0x000003;
const uint32_t login_resp = 0x000004;

const uint32_t pwm_set_duty_cycle = 0x000005;
const uint32_t pwm_set_duty_cycle_resp = 0x000006;

const uint32_t pwm_get_duty_cycle = 0x000007;
const uint32_t pwm_get_duty_cycle_resp = 0x000008;

const uint32_t pm2p5 = 0x000009;
const uint32_t pm2p5_resp = 0x00000a;

const uint32_t wind_speed = 0x00000b;
const uint32_t wind_speed_resp = 0x00000c;

const uint32_t wind_direction = 0x00000d;
const uint32_t wind_direction_resp = 0x00000e;

const uint32_t thl = 0x00000f;
const uint32_t thlresp = 0x000010;

}; // namespace net_code

#endif // global_h__
