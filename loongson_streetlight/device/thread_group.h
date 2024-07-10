#ifndef thread_group_h__
#define thread_group_h__


namespace std
{
class thread_group
{
public:
    thread_group( void ) = default;

    ~thread_group( void )
    {
        for ( auto* thread : threads_ )
        {
            delete thread;
        }

        threads_.clear();
    }

public:
    template <typename Function>
    thread* create_thread( Function&& fun )
    {
        lock_guard<mutex> lock( mutex_ );

        unique_ptr<thread> thread_ptr( new ( nothrow ) thread( fun ) );
        if ( thread_ptr == nullptr )
        {
            return nullptr;
        }

        threads_.emplace_back( thread_ptr.get() );

        return thread_ptr.release();
    }

    void add_thread( thread* thrd )
    {
        if ( thrd )
        {
            lock_guard<mutex> lock( mutex_ );

            threads_.emplace_back( thrd );
        }
    }

    void remove_thread( thread* thrd )
    {
        lock_guard<mutex> lock( mutex_ );

        auto iter = find( threads_.begin(), threads_.end(), thrd );
        if ( iter != threads_.end() )
        {
            threads_.erase( iter );
        }
    }

    void join_all( void )
    {
        lock_guard<mutex> lock( mutex_ );

        for ( auto* thread : threads_ )
        {
            if ( thread->joinable() )
            {
                thread->join();
            }
        }
    }

    size_t size( void ) const
    {
        lock_guard<mutex> lock( mutex_ );

        return threads_.size();
    }

    thread::id get_id( size_t index )
    {
        lock_guard<mutex> lock( mutex_ );
        if ( index > threads_.size() )
        {
            return thread::id{};
        }

        return threads_[index]->get_id();
    }

    bool is_this_thread_in( void )
    {
        thread::id id = this_thread::get_id();

        lock_guard<mutex> lock( mutex_ );

        for ( auto* thread : threads_ )
        {
            if ( thread->get_id() == id )
            {
                return true;
            }
        }
        return false;
    }

    bool is_thread_in( thread* thrd )
    {
        if ( thrd )
        {
            thread::id id = thrd->get_id();

            lock_guard<mutex> lock( mutex_ );

            for ( auto* thread : threads_ )
            {
                if ( thread->get_id() == id )
                {
                    return true;
                }
            }
        }

        return false;
    }

private:
    mutable mutex mutex_;
    vector<thread*> threads_;
};
} // namespace std

#endif // thread_group_h__
