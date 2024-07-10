#ifndef rs485_h__
#define rs485_h__

int rs485_init( const char* device );

bool read_pm2p5( double& data );
bool read_wind_speed( double& data );
bool read_thl( double& humidity, double& temperature, double& light );
bool read_wind_direction( std::string& direction, int& angle );

#endif // rs485_h__
