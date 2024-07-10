#ifndef pwm_h__
#define pwm_h__

extern unsigned int g_pwm;

//打开PWM接口
int pwm_export( unsigned int pwm );

//关闭PWM接口
int pwm_unexport( unsigned int pwm );

//使能pwm
int pwm_enable( unsigned int pwm );

//禁止使能pwm
int pwm_disable( unsigned int pwm );

//设置占空比（纳秒）
int pwm_period( unsigned int pwm, unsigned int period );

// 设置周期（纳秒）
int pwm_duty_cycle( unsigned int pwm, unsigned int duty_cycle );

// 配置
int pwm_config( unsigned int pwm, unsigned int period, unsigned int duty_cycle );

//设置极性
int pwm_polarity( unsigned int pwm, int polarity );

//设置极性
int pwm_read_duty_cycle( unsigned int pwm, int& polarity );

#endif // pwm_h__
