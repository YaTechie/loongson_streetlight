#ifndef api_session_h__
#define api_session_h__

class api_session : public std::enable_shared_from_this<api_session>
{
public:
    using ptr = std::shared_ptr<api_session>;
    using ptr_w = std::weak_ptr<api_session>;

    using key_type = uint64_t;
    using func_success_type = std::function<void( const char* content )>;
    using func_error_type = std::function<void( int result, int res_status )>;

public:
    api_session( void )
    {
    }

    api_session( func_success_type& fn_success, func_error_type& fn_error )
        : fn_success_( fn_success )
        , fn_error_( fn_error )
    {
    }

    api_session( func_success_type&& fn_success, func_error_type&& fn_error )
        : fn_success_( std::move( fn_success ) )
        , fn_error_( std::move( fn_error ) )
    {
    }

    ~api_session( void ) = default;

public:
    bool start( void );
    void stop( void );

    void trace( void );

    void add_headers( const char* header );
    bool add_content( void* content, std::size_t len );

    void success( void );
    void error( int result, int res_code );

public:
    inline void set_param( std::string_view param )
    {
        param_ = param;
    }

    inline std::string& get_param( void )
    {
        return param_;
    }

    inline uint8_t* get_content( void )
    {
        return content_;
    }

    inline std::size_t get_size( void )
    {
        return len_;
    }

    inline curl_slist* get_headers( void )
    {
        return headers_;
    }

    inline void set_in_sessions( bool in_sessions )
    {
        in_sessions_ = in_sessions;
    }

    inline bool get_in_sessions( void )
    {
        return in_sessions_;
    }

    inline key_type hash_key() const
    {
        return reinterpret_cast<key_type>( this );
    }

private:
    func_success_type fn_success_ = {};
    func_error_type fn_error_ = {};

    std::string param_ = {};
    uint8_t* content_ = nullptr;
    std::size_t len_ = 0;

    curl_slist* headers_ = nullptr;

    bool in_sessions_ = false;
};


#endif // api_session_h__