#include <cstring>
#include <vector>

#include "asio.hpp"

class TestStream{
  public:
    typedef asio::io_context::executor_type executor_type;

    TestStream(asio::io_context& io_context):  io_context_(io_context),
                                                length_(max_length),
                                                position_(0),
                                                next_write_length_(max_length){
        memset(data_, 0, max_length);
    }

    executor_type get_executor() ASIO_NOEXCEPT{
        return io_context_.get_executor();
    }

    void reset(size_t length = max_length){
        memset(data_, 0, max_length);
        length_ = length;
        position_ = 0;
        next_write_length_ = length;
    }

    void next_write_length(size_t length){
        next_write_length_ = length;
    }

    char* getBuf(){
        return data_;
    }

    size_t getLength(){
        return length_;
    }

    template <typename Iterator>
    bool check_buffers(Iterator begin, Iterator end, size_t length){
        if (length != position_)
            return false;

        Iterator iter = begin;
        size_t checked_length = 0;
        for (; iter != end && checked_length < length; ++iter){
            size_t buffer_length = asio::buffer_size(*iter);
            if (buffer_length > length - checked_length)
                buffer_length = length - checked_length;
            if (memcmp(data_ + checked_length, iter->data(), buffer_length) != 0)
                return false;
            checked_length += buffer_length;
        }

        return true;
    }

    template <typename Const_Buffers>
    bool check_buffers(const Const_Buffers& buffers, size_t length){
    return check_buffers(asio::buffer_sequence_begin(buffers),
        asio::buffer_sequence_end(buffers), length);
    }

    template <typename Const_Buffers>
    size_t write_some(const Const_Buffers& buffers){
        size_t n = asio::buffer_copy(
            asio::buffer(data_, length_) + position_,
        buffers, next_write_length_);
        position_ += n;
        return n;
    }

    template <typename Const_Buffers>
    size_t write_some(const Const_Buffers& buffers, asio::error_code& ec){
        ec = asio::error_code();
        return write_some(buffers);
    }

    template <typename Const_Buffers, typename Handler>
    void async_write_some(const Const_Buffers& buffers, ASIO_MOVE_ARG(Handler) handler){
        size_t bytes_transferred = write_some(buffers);
        asio::post(get_executor(),
            asio::detail::bind_handler(
            ASIO_MOVE_CAST(Handler)(handler),
            asio::error_code(), bytes_transferred));
    }

  private:
    asio::io_context& io_context_;
    enum { max_length = 8192 };
    char data_[max_length];
    size_t length_;
    size_t position_;
    size_t next_write_length_;
};