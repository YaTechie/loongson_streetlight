#ifndef net_timer_h__
#define net_timer_h__

class net_timer
{
public:
    net_timer( bool active, int64_t delay );
    net_timer( bool active, int64_t delay, int count );
    net_timer( bool active, int64_t delay, std::function<void( void )> fun );
    net_timer( bool active, int64_t delay, int count, std::function<void( void )> fun );
    ~net_timer() = default;

public:
    void start( void );
    void start( int64_t tick );

    void stop( void );

public:
    inline void set_active( bool active );
    inline bool get_active( void );

    inline void set_delay( int64_t delay );
    inline int64_t get_delay( void );

    inline void set_count( int count );
    inline int get_count( void );

    inline void set_param( uint64_t param );
    inline uint64_t get_param( void );

    inline void set_active_count( int active_count );
    inline int get_active_count( void );

    inline void set_tick( int64_t tick );
    inline int64_t get_tick( void );
    inline void add_tick( int64_t tick );

    inline int64_t get_left( void );
    inline int64_t get_left_second( void );

public:
    bool action( int64_t time );

protected:
    std::atomic<bool> active_;

    int64_t delay_;
    int count_;
    uint64_t param_;

    std::function<void( void )> fun_;

    volatile int active_count_;
    volatile int64_t tick_;
};

inline void net_timer::set_active( bool active )
{
    active_ = active;
}

inline bool net_timer::get_active( void )
{
    return active_;
}

inline void net_timer::set_delay( int64_t delay )
{
    delay_ = delay;
}

inline int64_t net_timer::get_delay( void )
{
    return delay_;
}

inline void net_timer::set_count( int count )
{
    count_ = count;
}
inline int net_timer::get_count( void )
{
    return count_;
}

inline void net_timer::set_param( uint64_t param )
{
    param_ = param;
}

inline uint64_t net_timer::get_param( void )
{
    return param_;
}

inline void net_timer::set_active_count( int active_count )
{
    active_count_ = active_count;
}

inline int net_timer::get_active_count( void )
{
    return active_count_;
}

inline void net_timer::set_tick( int64_t tick )
{
    tick_ = tick;
}

inline int64_t net_timer::get_tick( void )
{
    return tick_;
}

inline void net_timer::add_tick( int64_t tick )
{
    tick_ += tick;
}

inline int64_t net_timer::get_left( void )
{
    if ( active_ )
    {
        return ( std::max )( delay_ - tick_, static_cast<int64_t>( 0 ) );
    }

    return 0;
}

inline int64_t net_timer::get_left_second( void )
{
    return get_left() / 1000;
}

#endif // net_timer_h__
