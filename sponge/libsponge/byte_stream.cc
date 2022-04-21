#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):
	cap(capacity),
	is_eof(false),
	total_write(0),
	total_read(0),
	buf(){}

size_t ByteStream::write(const string &data) {
	if(data.size() == 0)
		return 0;
	size_t byte_write = data.size() > (cap - buf.size()) ?
						cap - buf.size() : data.size();
	for(size_t i = 0; i < byte_write; i++){
		this -> buf.emplace_back(data[i]);
	}
	total_write += byte_write;
    return byte_write;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
	string s = "";
	for(size_t i = 0; i < len; i++){
		s.append(1, buf[i]);
	}
    return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
	total_read += len;
	this -> buf.erase(buf.begin(), buf.begin()+len);
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
	size_t byte_read = len > buf.size() ? buf.size() : len;
	string res = peek_output(byte_read);
	pop_output(byte_read);
    return res;
}

void ByteStream::end_input() {is_eof = true;}

bool ByteStream::input_ended() const { return is_eof; }

size_t ByteStream::buffer_size() const { return buf.size(); }

bool ByteStream::buffer_empty() const { return buf.size() == 0; }

bool ByteStream::eof() const { return is_eof && buf.empty(); }

size_t ByteStream::bytes_written() const { return total_write; }

size_t ByteStream::bytes_read() const { return total_read; }

size_t ByteStream::remaining_capacity() const { return cap - buf.size(); }
