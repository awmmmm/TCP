#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    ByteStream::capacity_ = capacity;
    ByteStream::remaining_capacity_ = capacity;
    ByteStream::end_input_ = false;
    ByteStream::eof_ = false;
    ByteStream::bytes_written_ = 0;
    ByteStream::bytes_read_ = 0;
}

size_t ByteStream::write(const string &data) {
    if (ByteStream::input_ended())
        return 0;
    size_t data_size = data.size();

    if (data_size <= ByteStream::remaining_capacity_) {
        ByteStream::buffer = ByteStream::buffer + data;
        ByteStream::remaining_capacity_ -= data_size;
        ByteStream::bytes_written_ += data_size;
        // end_input();
        return data_size;
    }

    size_t temp = ByteStream::remaining_capacity_;
    ByteStream::buffer = ByteStream::buffer + data.substr(0, temp);
    ByteStream::remaining_capacity_ = 0;
    ByteStream::bytes_written_ += temp;

    return temp;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (len >= ByteStream::buffer.size())
        return ByteStream::buffer;
    return ByteStream::buffer.substr(0, len);

    // DUMMY_CODE(len);
    // return {};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (len >= ByteStream::buffer.size()) {
        if (input_ended())
            ByteStream::eof_ = true;
        ByteStream::remaining_capacity_ += ByteStream::buffer.size();
        ByteStream::bytes_read_ += ByteStream::buffer.size();
        ByteStream::buffer = "";
        return;
    }

    ByteStream::buffer = ByteStream::buffer.substr(len);
    ByteStream::bytes_read_ += len;
    ByteStream::remaining_capacity_ += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    // if (input_ended() && len >= ByteStream::buffer.size())
    //     ByteStream::eof_ = true;
    string res = ByteStream::peek_output(len);
    pop_output(len);

    return res;
}

void ByteStream::end_input() {
    ByteStream::end_input_ = true;
    if (ByteStream::buffer_empty())
        ByteStream::eof_ = true;
}

bool ByteStream::input_ended() const { return ByteStream::end_input_; }

size_t ByteStream::buffer_size() const { return ByteStream::buffer.size(); }

bool ByteStream::buffer_empty() const { return ByteStream::buffer.empty(); }

bool ByteStream::eof() const { return ByteStream::eof_; }

size_t ByteStream::bytes_written() const { return ByteStream::bytes_written_; }

size_t ByteStream::bytes_read() const { return ByteStream::bytes_read_; }

size_t ByteStream::remaining_capacity() const { return ByteStream::remaining_capacity_; }
