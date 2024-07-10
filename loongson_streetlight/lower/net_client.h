#ifndef net_client_h__
#define net_client_h__

class net_client
{
public:
    explicit net_client( void )
        : iopool_( 2 )
        , client_( std::list<asio2::io_t*>{ &iopool_.get( 0 ) } )
        , nt_loop_( false, 1 * 1000, std::bind( &net_client::_on_loop_timer, this ) )
        , nt_t_( true, 2 * 1000, std::bind( &net_client::_on_t_timer, this ) ){};

    ~net_client( void ) = default;

public:
    inline static net_client& get_instance( void )
    {
        static net_client instance;
        return instance;
    }

public:
    bool start( std::string_view path );
    void stop( void );

private:
    void _loop( void );
    void _on_loop_timer( void );
    void _on_t_timer( void );

private:
    bool _init( std::string_view path );
    bool _connect( void );
    bool _send( int commond, const char* content );

private:
    std::atomic<bool> run_ = false;
    std::thread_group threads_ = {};

private:
    asio2::iopool iopool_;
    asio2::ws_client client_;
    net_timer nt_loop_;
    net_timer nt_t_;

    std::string host_ = "127.0.0.1";
    std::string port_ = "80";
};

#endif // net_client_h__
