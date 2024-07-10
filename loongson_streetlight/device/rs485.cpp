#include "stdafx.h"

#include "rs485.h"

#if !defined( WIN32 ) && !defined( _WIN32 ) && !defined( _WIN64 ) && !defined( _WINDOWS_ )
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif


int rs485_fd = -1;


bool rs485_send( const unsigned char* data, int len )
{
    if ( rs485_fd  == -1 )
    {
        return false;
    }

    if ( write( rs485_fd, data, len ) != len )
    {
        return false;
    }

    return true;
}

bool rs485_read( unsigned char* buf, int* len )
{
    if ( rs485_fd == -1 )
    {
        return false;
    }

    int r = (int)read( rs485_fd, buf, *len );
    if ( r < 0 )
    {
        return false;
    }

    *len = r;

    return true;
}

int crc16( const unsigned char* buf, int len )
{
    int crc = 0xFFFF;
    int i, j;
    for ( i = 0; i < len; i++ )
    {
        crc ^= buf[i];
        for ( j = 0; j < 8; j++ )
        {
            if ( crc & 0x0001 )
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

std::unique_ptr<unsigned char[]> read_data_by_command( unsigned char device, unsigned char cmd, int reg, int reg_len )
{
    unsigned char rs_buf[8] = {
        device,
        cmd,
        static_cast<unsigned char>( reg >> 8 ),
        static_cast<unsigned char>( reg & 0xFF ),
        static_cast<unsigned char>( reg_len >> 8 ),
        static_cast<unsigned char>( reg_len & 0xFF )
    };

    // 计算校验码
    int crc = crc16( rs_buf, 6 );
    rs_buf[6] = crc & 0xFF; // 校验码低位
    rs_buf[7] = crc >> 8;   // 校验码高位

    // 发送问询帧数据
    rs485_send( rs_buf, 8 );

    // 接收应答帧数据
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) ); // 100ms 等待数据接收完整，根据实际情况调整延时时间

    unsigned char buf[64];
    memset( buf, 0, sizeof( buf ) );

    int len = sizeof( buf );
    rs485_read( buf, &len ); // 接收数据存放在rx_buf中，数据长度存放在len中

    // 至少接收到7个字节的数据
    if ( len < 7 )
    {
        return nullptr;
    }

    if ( buf[0] != device || buf[1] != cmd )
    {
        return nullptr;
    }

    // 读取数据
    int data_len = buf[2];

    std::unique_ptr<unsigned char[]> r( new unsigned char[data_len] );
    for ( int i = 0; i < data_len; i++ )
    {
        r[i] = buf[3 + i];
    }

    return r;
}

int rs485_init( [[maybe_unused]] const char* device )
{
#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )
    return 0;
#else
    struct termios tty = {};

    rs485_fd = open( device, O_RDWR | O_NOCTTY | O_NONBLOCK );
    if ( rs485_fd < 0 )
    {
        return -1;
    }

    memset( &tty, 0, sizeof( tty ) );
    if ( tcgetattr( rs485_fd, &tty ) != 0 )
    {
        return -1;
    }

    int baud_rate = B4800;

    // 设置波特率
    cfsetospeed( &tty, baud_rate );
    cfsetispeed( &tty, baud_rate );

    tty.c_cflag |= ( CLOCAL | CREAD );
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_iflag |= IGNBRK;
    tty.c_iflag &= ~( IXON | IXOFF | IXANY );
    tty.c_oflag &= ~OPOST;

    // 非正规模式设置
    tty.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG );
    tty.c_iflag &= ~( INLCR | PARMRK | ISTRIP | INPCK | ICRNL );

    //设置等待时间和最小接收字符
    tty.c_cc[VTIME] = 1; //读取一个字符等待1*(1/10)s
    tty.c_cc[VMIN] = 1;  //读取字符的最少个数为1

    if ( tcsetattr( rs485_fd, TCSANOW, &tty ) != 0 )
    {
        return -1;
    }

    // 设置串口的非阻塞读取
    int flags = fcntl( rs485_fd, F_GETFL, 0 );
    if ( flags == -1 )
    {
        return -1;
    }

    flags |= O_NONBLOCK;
    if ( fcntl( rs485_fd, F_SETFL, flags ) == -1 )
    {
        return -1;
    }

    return 0;
#endif
}

bool read_pm2p5( double& data )
{
    std::vector<unsigned char> cmd;

    const std::string pm25_cmd = "030300080001";

    for ( size_t i = 0; i < pm25_cmd.length(); i += 2 )
    {
        std::string byteString = pm25_cmd.substr( i, 2 );

        unsigned char byte = (unsigned char)strtol( byteString.c_str(), nullptr, 16 );
        cmd.push_back( byte );
    }

    const unsigned char device = cmd[0];
    const unsigned char command = cmd[1];
    const unsigned char reg = cmd[2] << 8 | cmd[3];
    const unsigned char reg_len = cmd[4] << 8 | cmd[5];

    auto res485 = read_data_by_command( device, command, reg, reg_len );
    if ( res485 == nullptr )
    {
        return false;
    }

    int res_data = ( res485[0] << 8 ) | res485[1];

    data = res_data;

    return true;
}

bool read_wind_speed( double& data )
{
    std::vector<unsigned char> cmd;

    const std::string wind_speed_cmd = "010300000001";

    for ( size_t i = 0; i < wind_speed_cmd.length(); i += 2 )
    {
        std::string byteString = wind_speed_cmd.substr( i, 2 );
        unsigned char byte = (unsigned char)strtol( byteString.c_str(), nullptr, 16 );
        cmd.push_back( byte );
    }

    const unsigned char device = cmd[0];
    const unsigned char command = cmd[1];
    const unsigned char reg = cmd[2] << 8 | cmd[3];
    const unsigned char reg_len = cmd[4] << 8 | cmd[5];

    auto res485 = read_data_by_command( device, command, reg, reg_len );
    if ( res485 == nullptr )
    {
        return false;
    }

    // 解析数据
    int res_data = ( res485[0] << 8 ) | res485[1];

    data = res_data / 10.0; // 风速值 = 读取到的值 / 10.0

    return true;
}

bool read_thl( double& humidity, double& temperature, double& light )
{
    std::vector<unsigned char> cmd;

    const std::string thl_cmd = "020300000004";

    for ( size_t i = 0; i < thl_cmd.length(); i += 2 )
    {
        std::string byteString = thl_cmd.substr( i, 2 );
        unsigned char byte = (unsigned char)strtol( byteString.c_str(), nullptr, 16 );
        cmd.push_back( byte );
    }

    const unsigned char deviceAddr = cmd[0];
    const unsigned char command = cmd[1];
    const unsigned char regAddr = cmd[2] << 8 | cmd[3];
    const unsigned char regLen = cmd[4] << 8 | cmd[5];

    auto res485 = read_data_by_command( deviceAddr, command, regAddr, regLen );
    if ( res485 == nullptr )
    {
        return false;
    }

    /**
     * 应答帧 地址码 功能码 字节数 湿度值 温度值 光照高位 光照低位 校验码
     * 0x01 0x03 0x08 0x02 0x92 0x80 0x65 0x00 0x03 0x0D 0x40 0x01 0x6F
     */

    // 解析数据
    int res_humidity = ( res485[0] << 8 ) | res485[1];
    int res_temperature = ( res485[2] << 8 ) | res485[3];
    int res_light = ( res485[4] << 8 ) | res485[5];

    humidity = res_humidity / 10.0;       // 湿度值 = 读取到的值 / 10.0
    temperature = res_temperature / 10.0; // 温度值 = 读取到的值 / 10.0
    light = res_light;                    // 光照值 = 读取到的值

    return true;
}

bool read_wind_direction( std::string& direction, int& angle )
{
    std::vector<unsigned char> cmd;

    const std::string wind_direction_cmd = "040300000002";

    for ( size_t i = 0; i < wind_direction_cmd.length(); i += 2 )
    {
        std::string byteString = wind_direction_cmd.substr( i, 2 );
        unsigned char byte = (unsigned char)strtol( byteString.c_str(), nullptr, 16 );
        cmd.push_back( byte );
    }

    const unsigned char device = cmd[0];
    const unsigned char command = cmd[1];
    const unsigned char reg = cmd[2] << 8 | cmd[3];
    const unsigned char reg_len = cmd[4] << 8 | cmd[5];

    auto res485 = read_data_by_command( device, command, reg, reg_len );
    if ( res485 == nullptr )
    {
        return false;
    }

    // 解析数据
    int direction_value = ( res485[0] << 8 ) | res485[1];
    angle = ( res485[2] << 8 ) | res485[3];

    // 根据采集值（0-7档）来确定风向
    switch ( direction_value )
    {
        case 0:
            direction = "North"; // 北 North
            break;
        case 1:
            direction = "Northeast"; // 东北 Northeast
            break;
        case 2:
            direction = "East"; // 东 East
            break;
        case 3:
            direction = "Southeast"; // 东南 Southeast
            break;
        case 4:
            direction = "South"; // 南 South
            break;
        case 5:
            direction = "Southwest"; // 西南 Southwest
            break;
        case 6:
            direction = "West"; // 西 West
            break;
        case 7:
            direction = "Northwest"; // 西北 Northwest
            break;
        default:
            direction = "Unknown"; // 未知方向 Unknown
            break;
    }

    // 返回风向描述字符串
    return true;
}
