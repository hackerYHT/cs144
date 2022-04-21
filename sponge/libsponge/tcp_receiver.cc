#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader header = seg.header();
    //处于listen状态，只接收syn || 如果已经接收到了syn，就拒绝接收其他的syn
    if((!header.syn && !_syn) || (header.syn && _syn)) return;
    WrappingInt32 seqno(header.seqno);
    if(seg.header().syn){
        _isn = seqno;
        _syn = true;
    }
    if(_syn && header.fin) _fin = true;
    uint64_t index = header.syn ? 0 : 
        unwrap(seqno, _isn, _reassembler.stream_out().bytes_written())-1;
    _reassembler.push_substring(seg.payload().copy(), index, header.fin); 
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if(!_syn) return {};
    WrappingInt32 res = WrappingInt32(wrap(_reassembler.stream_out().bytes_written(), _isn)); 
    return _reassembler.stream_out().input_ended() ? res + 2 : res + 1;//不确定类型是否需要转换
}

size_t TCPReceiver::window_size() const { 
    return _reassembler.stream_out().bytes_read() + _capacity - _reassembler.stream_out().bytes_written();
}
