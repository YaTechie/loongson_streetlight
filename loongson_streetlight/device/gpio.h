#ifndef gpio_h__
#define gpio_h__

#define MAP_SIZE 0x10000
#define REG_BASE 0x1fe10000

#define GPIO_EN 0x500
#define GPIO_OUT 0x510
#define GPIO_IN 0x520
#define I2C0 0x1fe1f000
#define I2C1 0x1fe1f800

extern unsigned char* map_base;

//init
int gpio_init( void );
int gpio_enable( int gpio_num, int val );
int gpio_close( void );
int gpio_write( int gpio, int val );
int gpio_read( int gpio_num );

#endif // gpio_h__
