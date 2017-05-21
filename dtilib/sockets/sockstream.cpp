/*
Copyright (c) 2005 by the Danish Technological Institute.

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/
#include <dti/sockets/sockstream.h>
#include <cstdio>
#include <iostream>

using namespace dti::sockets;


template <typename Trait>
class tcp_device_wrapper : public dti::stream::detail::device_wrapper<Trait> {
	typedef typename dti::stream::detail::device_wrapper<Trait>::char_type char_type;

  private:
	tcp_socket mysock;
	bool abortme;

  public:
	tcp_device_wrapper() : abortme(false) {}
	tcp_device_wrapper(tcp_socket& s) : mysock(s), abortme(false) {}
		
	int read(char_type* buf, std::streamsize len) {
		if (abortme)
			return 0;
		return mysock.recv((void *)buf, len*sizeof(char_type));
	}

	int write(const char_type* buf, std::streamsize len) {
		if (abortme)
			return len;
		return mysock.send((const void*)buf, len*sizeof(char_type));
	}

	bool sync() {
		return true;
	}

	std::streamsize showmanyc() { 
		return mysock.fionread()/sizeof(char_type);
	}
	
	void abort() {
		abortme = true;
		mysock.abort();
	}

	void clear_abort() {
		abortme = false;
	}

	// Uses default implementation of "seekoff"
	// Uses default implementation of "seekpos"
};

template <typename Trait>
void basic_tcpstreambuf<Trait>::abort()
{
	(dynamic_cast<tcp_device_wrapper<Trait>*>(this->dw))->abort();
}

template <typename Trait>
void basic_tcpstreambuf<Trait>::clear_abort()
{
	(dynamic_cast<tcp_device_wrapper<Trait>*>(this->dw))->clear_abort();
}

template <typename Trait>
basic_tcpstreambuf<Trait>::basic_tcpstreambuf()
: dti::stream::detail::streambuf_impl<Trait, dti::stream::detail::dual_buffer<typename Trait::char_type> >(new tcp_device_wrapper<Trait>)
{
}

template <typename Trait>
basic_tcpstreambuf<Trait>::basic_tcpstreambuf(tcp_socket& s)
	: dti::stream::detail::streambuf_impl<Trait, dti::stream::detail::dual_buffer<typename Trait::char_type> >(new tcp_device_wrapper<Trait>(s))
{
}



template <typename BS>
basic_tcpstream<BS>::basic_tcpstream()
	: BS(0)
{
	this->init(&tcpbuf);
}

template <typename BS>
basic_tcpstream<BS>::basic_tcpstream(tcp_socket& s)
	: BS(0),
	  tcpbuf(s)
{
	this->init(&tcpbuf);
}


// TODO: The following instatiations should be put in differenct library files

// Instatiate the interesting classes
template class dti::sockets::basic_tcpstream< std::istream >;	// itcpstream
template class dti::sockets::basic_tcpstream< std::ostream >;	// otcpstream
template class dti::sockets::basic_tcpstream< std::iostream >;	// tcpstream


#ifdef _WIN32
template class dti::sockets::basic_tcpstream< std::wistream >;	// witcpstream
template class dti::sockets::basic_tcpstream< std::wostream >;	// wotcpstream
template class dti::sockets::basic_tcpstream< std::wiostream >;	// wtcpstream
#endif