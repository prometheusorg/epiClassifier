// -*- c++ -*-
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

#include "../../include/dti/sockets/sock.h"
#ifdef _WIN32
#	include <winsock2.h>
#	include <windows.h>
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <unistd.h>
#	include <errno.h>
#	include <netdb.h>
#	include <sys/ioctl.h>
#endif

#include <stdio.h>
#include <cstdio>
#include <cstring>

#ifndef _WIN32_WCE
#include <sstream>
#endif

using std::string;
using namespace dti::sockets;

#if defined(UNICODE) || defined(_UNICODE)
	typedef wchar_t chichar_t;
#	define CHIT(txt) L##txt
#else
	typedef char chichar_t;
#	define CHIT(txt) txt
#endif

#ifdef _WIN32

//*****************************  INITWINSOCK  ********************************
// Windows requires that WinSock be initialized through a call to WSAStartup.
// This class hides this from the user by creating a global object with a
// constructor that is called before main().
//----------------------------------------------------------------------------


#ifndef _WIN32_WCE
#include <iostream>
#endif

namespace {
	// Initialize Winsock

	class initwinsock {
	  public:
		initwinsock() {
			WSADATA wsaData;
			int err = WSAStartup(MAKEWORD(2,2), &wsaData);
			if (err)
#ifdef _WIN32_WCE
				::fprintf(stderr,"Cannot start Winsock. Error %d.\n", WSAGetLastError());
#else
				std::cerr << "Cannot start Winsock. Error "
						  << WSAGetLastError() << "." << std::endl;
#endif
		}
	} initwinsockobj;
}
#endif


//******************************  SOCK_ERROR  ********************************
// Socket error class
//----------------------------------------------------------------------------

sock_error::sock_error(const string& arg)
	: std::runtime_error(arg)
{
}

//****************************  SOCK_ERROR_EX  *******************************
// Extended socket error class
//----------------------------------------------------------------------------

sock_error_ex::sock_error_ex(const string& arg)
	: sock_error(arg)
{
#ifdef _WIN32
	syserr = WSAGetLastError();

	chichar_t buf[256];

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, syserr, 0, buf, sizeof buf, 0)<=0)
		systext = CHIT("Unknown error");
	else
		systext = buf;
#else
	syserr = errno;
	if (syserr>0 && syserr<sys_nerr)
		systext = sys_errlist[syserr];
	else
		systext = "Unknown error";
#endif
}

int sock_error_ex::whaterr() const
{
	return syserr;
}

#ifdef _WIN32_WCE
std::wstring sock_error_ex::whatsystext() const
#else
string sock_error_ex::whatsystext() const
#endif
{
	return systext;
}


//********************************  IPADDR  **********************************
// IP address class
//----------------------------------------------------------------------------

bool ipaddr::valid_ip(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3)
{
	return a0<256 && a1<256 && a2<256 && a3<256;
}

ipaddr::ipaddr()
{
	a[0]=a[1]=a[2]=a[3]=0;
}

ipaddr::ipaddr(const string& s)
{
	// It would have been nice if we could have used inet_aton here, but it is
	// not supported in Windows

	if (::sscanf(s.c_str(),"%u.%u.%u.%u",&a[0],&a[1],&a[2],&a[3])!=4 ||
		!valid_ip(a[0], a[1], a[2], a[3]))
		throw sock_error("Illegal IP address");
}

ipaddr::ipaddr(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3)
{
	a[0] = a0;
	a[1] = a1;
	a[2] = a2;
	a[3] = a3;

	if (!valid_ip(a[0], a[1], a[2], a[3]))
		throw sock_error("Illegal IP address");
}

ipaddr::ipaddr(unsigned int addr)
{
	a[0] = addr&0xff;
	a[1] = (addr>>8)&0xff;
	a[2] = (addr>>16)&0xff;
	a[3] = (addr>>24)&0xff;

	// There is no need to validate this IP address. Its components are always <256.
}

string ipaddr::to_string() const
{
#ifdef _WIN32_WCE
	char buf[16];				// Long enough to hold "255.255.255.255" plus a terminating null
	::sprintf(buf,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
	return buf;
#else
	std::ostringstream os;
	os << a[0] << "." << a[1] << "." << a[2] << "." << a[3];
	return os.str();
#endif
}

unsigned int ipaddr::to_uint() const
{
	return (a[3]<<24) | (a[2]<<16) | (a[1]<<8) | a[0];
}

bool ipaddr::operator==(const ipaddr& rhs) const
{
	return a[0]==rhs.a[0] && a[1]==rhs.a[1] && a[2]==rhs.a[2] && a[3]==rhs.a[3];
}

bool ipaddr::operator!=(const ipaddr& rhs) const
{
	return !(*this==rhs);
}


//*******************************  ENDPOINT  *********************************
// Connection endpoint class
//----------------------------------------------------------------------------

endpoint::endpoint(int port)
	: spec_as_ip(true),
	  DNS_lookup(true),
	  hostname("0.0.0.0"),
	  ip_address(0,0,0,0),
 	  portno(port)
{
}

void endpoint::initme(const string& ip, int port)
{
	unsigned int a0, a1, a2, a3;

	if (::sscanf(ip.c_str(),"%u.%u.%u.%u",&a0,&a1,&a2,&a3)==4) {
		spec_as_ip = true;
		ip_address = ipaddr(a0,a1,a2,a3);
		DNS_lookup = false;
	}
	else {
		struct hostent *he = ::gethostbyname(ip.c_str());

		spec_as_ip = false;

		if (!he)
			throw sock_error_ex("Unknown hostname");

		for (unsigned char **cp = reinterpret_cast<unsigned char**>(he->h_addr_list); *cp; cp++)
			ipaddresses.push_back(ipaddr((*cp)[0], (*cp)[1], (*cp)[2], (*cp)[3]));

		hostname = ip;
		ip_address = ipaddresses[0];
		DNS_lookup = true;
	}
	portno = port;
}

endpoint::endpoint(const string& ip, int port)
{
	initme(ip,port);
}

endpoint::endpoint(const ipaddr& ip, int port)
	: spec_as_ip(true),
	  DNS_lookup(false),
	  ip_address(ip),
	  portno(port)
{
}

int endpoint::decode_service(const string& service)
{
	int serv;

	if (::sscanf(service.c_str(),"%u",&serv)==1)
		return serv;
	else {
		servent* se = ::getservbyname(service.c_str(),"TCP");

		if (!se)
			throw sock_error_ex("Unknown service name");

		return ntohs(se->s_port);
	}
}

endpoint::endpoint(const string& ip, const string& service)
{
	initme(ip,decode_service(service));
}

endpoint::endpoint(const ipaddr& ip, const string& service)
	: spec_as_ip(true),
	  DNS_lookup(false),
	  ip_address(ip)
{
	portno = decode_service(service);
}

string endpoint::get_ipstring() const
{
	return ip_address.to_string();
}

ipaddr endpoint::get_ipaddr() const
{
	return ip_address;
}

string endpoint::get_host() const
{
	if (spec_as_ip && !DNS_lookup) {
		unsigned int ul = ip_address.to_uint();
		struct hostent *he = ::gethostbyaddr((const char*)&ul, 4, AF_INET);

		if (!he)
			hostname = "";
		else
			hostname = he->h_name;

		DNS_lookup = true;
	}
	return hostname;
}

int endpoint::get_port() const
{
	return portno;
}

string endpoint::get_service() const
{
	servent* se = ::getservbyport(htons(portno),"TCP");

	if (!se)
		return "";
	else
		return se->s_name;
}

string endpoint::get_ipport() const
{
#ifdef _WIN32_WCE
	char buf[80];
	::sprintf(buf, "%s:%d", ip_address.to_string().c_str(), portno);
	return buf;
#else
	std::ostringstream os;
	os << ip_address << ":" << portno;
	return os.str();
#endif
}

string endpoint::get_hostport() const
{
#ifdef _WIN32_WCE
	char buf[80];
	::sprintf(buf, "%s:%d", get_host().c_str(), portno);
	return buf;
#else
	std::ostringstream os;
	os << get_host() << ":" << portno;
	return os.str();
#endif
}

bool endpoint::operator==(const endpoint& rhs) const
{
	return ip_address==rhs.ip_address && portno==rhs.portno;
}

bool endpoint::operator!=(const endpoint& rhs) const
{
	return !(*this==rhs);
}


//*****************************  SOCK_WRAPPER  *******************************
// All-private wrapper around a socket handle and I/O formatters.
// This is a pseudo-singleton: Only one instance exists per open connection,
// regardless of how often the referring objects are copied.
//----------------------------------------------------------------------------

detail::sock_wrapper::sock_wrapper(socket_handle s)
	: sockhand(s),
	  refcount(1),
	  outf(0),
	  inf(0)
#ifdef _WIN32
	  , abort_recv_event(0), abort_send_event(0)
#endif
#ifdef __GNUC__
	  , abort_recv_socket(-1), abort_send_socket(-1)
#endif	
{
}

detail::sock_wrapper::~sock_wrapper()
{
	close_if_open();

	delete outf;
	delete inf;
}

void detail::sock_wrapper::inc_refcount()
{
	++refcount;
}

int detail::sock_wrapper::dec_refcount()
{
	return --refcount;
}

void detail::sock_wrapper::open()
{
	if (is_open())
		throw sock_error("Socket already open");

	sockhand = socket(PF_INET,SOCK_STREAM,0);

	if (sockhand==-1)
		throw sock_error_ex("Cannot create socket");
}

bool detail::sock_wrapper::is_open() const
{
	return sockhand!=-1;
}

void detail::sock_wrapper::test_open() const
{
	if (!is_open())
		throw sock_error("Socket not open");
}

void detail::sock_wrapper::close_if_open()
{
	if (is_open()) {
#ifdef _WIN32
		closesocket(sockhand);
#else
		close(sockhand);
#endif
		sockhand = -1;
	}
}

socket_handle detail::sock_wrapper::get_socket_handle() const
{
	return sockhand;
}

#ifdef _WIN32
//============================  recv_abortable  ==============================
// Emulates a recv using overlapped I/O. However, it leaves an abort_recv_event
// in the sock_wrapper. By signalling this event, the I/O operation is
// cancelled.
//----------------------------------------------------------------------------
int detail::sock_wrapper::recv_abortable(void *buf, std::size_t len, int flags)
{
	WSAEVENT io_event = 0;
	DWORD val = 0;

	try {
		io_event = CreateEvent(0, false, false, 0);
		abort_recv_event = CreateEvent(0, false, false, 0);
		if (!io_event || !abort_recv_event)
			throw sock_error("Cannot create event handle");

		WSABUF wb = {len, (char*)buf};

		WSAOVERLAPPED wo;
		wo.hEvent = io_event;

		WSAEVENT waitfor[2] = { io_event, abort_recv_event };

		DWORD fl = flags;

		if (WSARecv(sockhand, &wb, 1, &val, &fl, &wo, 0)!=0) {
			// I/O did not complete immediately, or we had an error

			if (WSAGetLastError()!=WSA_IO_PENDING) {
				CloseHandle(abort_recv_event);
				CloseHandle(io_event);
				throw sock_error_ex("Cannot receive from socket");
			}

			// I/O did not complete immediately. Wait for completion or abort

			switch (WSAWaitForMultipleEvents(2, waitfor, FALSE, WSA_INFINITE, FALSE)) {
			  case WSA_WAIT_EVENT_0:
					// I/O completed
					if (!WSAGetOverlappedResult(sockhand, &wo, &val, TRUE, &fl))
						throw sock_error_ex("Cannot receive from socket");
					break;

			  case WSA_WAIT_EVENT_0+1:
					// Abort request
					if (!CancelIo((HANDLE)sockhand))
						throw sock_error("Cannot cancel I/O request");
					break;

			  case WSA_WAIT_FAILED:
					throw sock_error_ex("Cannot wait for I/O completion");
					break;
			}
		}
	}
	catch (...) {
		if (abort_recv_event) {
			CloseHandle(abort_recv_event);
			abort_recv_event = 0;
		}
		if (io_event)
			CloseHandle(io_event);
		throw;
	}

	CloseHandle(abort_recv_event);
	CloseHandle(io_event);
	abort_recv_event = 0;
	return val;
}

//============================  send_abortable  ==============================
// Emulates a send using overlapped I/O. However, it leaves an abort_send_event
// in the sock_wrapper. By signalling this event, the I/O operation is
// cancelled.
//----------------------------------------------------------------------------
int detail::sock_wrapper::send_abortable(const void *buf, std::size_t len, int flags)
{
	WSAEVENT io_event = 0;
	DWORD val = 0;

	try {
		io_event = CreateEvent(0, false, false, 0);
		abort_send_event = CreateEvent(0, false, false, 0);
		if (!io_event || !abort_send_event)
			throw sock_error("Cannot create event handle");

		WSABUF wb = {len, (char*)buf};

		WSAOVERLAPPED wo;
		wo.hEvent = io_event;

		WSAEVENT waitfor[2] = { io_event, abort_send_event };

		if (WSASend(sockhand, &wb, 1, &val, flags, &wo, 0)!=0) {
			// I/O did not complete immediately, or we had an error

			if (WSAGetLastError()!=WSA_IO_PENDING) {
				CloseHandle(abort_send_event);
				CloseHandle(io_event);
				throw sock_error_ex("Cannot send to socket");
			}

			// I/O did not complete immediately. Wait for completion or abort

			switch (WSAWaitForMultipleEvents(2, waitfor, FALSE, WSA_INFINITE, FALSE)) {
			  case WSA_WAIT_EVENT_0:
					// I/O completed
					DWORD fl;
					if (!WSAGetOverlappedResult(sockhand, &wo, &val, TRUE, &fl))
						throw sock_error_ex("Cannot send to socket");
					break;

			  case WSA_WAIT_EVENT_0+1:
					// Abort request
					if (!CancelIo((HANDLE)sockhand))
						throw sock_error("Cannot cancel I/O request");
					break;

			  case WSA_WAIT_FAILED:
					throw sock_error_ex("Cannot wait for I/O completion");
					break;
			}
		}
	}
	catch (...) {
		if (abort_send_event) {
			CloseHandle(abort_send_event);
			abort_send_event = 0;
		}
		if (io_event)
			CloseHandle(io_event);
		throw;
	}

	CloseHandle(abort_send_event);
	CloseHandle(io_event);
	abort_send_event = 0;
	return val;
}
#endif

#ifdef __GNUC__
//============================  recv_abortable  ==============================
// Emulates a recv. Waits for data on either the TCP socket or the
// abort_recv_socket.
//----------------------------------------------------------------------------
int detail::sock_wrapper::recv_abortable(void *buf, std::size_t len, int flags)
{
	unsigned long count;

	if (ioctl(sockhand, FIONREAD, &count) < 0)
		throw sock_error_ex("Cannot get socket data count");

	if (count>0)
		// There is already data available - no need to allow aborting
		return ::recv(sockhand, (char*)buf, len, flags);


	// We must allow aborting
	int handlepair[2];

	if (socketpair(PF_UNIX, SOCK_DGRAM, 0, handlepair))
		throw sock_error_ex("Cannot create abort socket pair");

	abort_recv_socket = handlepair[0];

	fd_set readset;

	FD_ZERO(&readset);
	FD_SET(sockhand, &readset);
	FD_SET(handlepair[1], &readset);

	int maxhandle = std::max(sockhand, handlepair[1]) + 1;

	int retval=0;

	try {
		if (select(maxhandle, &readset, 0, 0, 0) > 0) {
			if (FD_ISSET(sockhand, &readset))	/* Is TCP socket readable? */
				retval = ::recv(sockhand, (char*)buf, len, flags);
			else if (FD_ISSET(handlepair[1], &readset))	{ /* Is abort socket readable? */
				char ch;
				::recv(handlepair[1], &ch, sizeof(char), 0);
				// We ignore what we read. It must be an abort command
			}
		}
		else
			throw sock_error_ex("Cannot select input");
	}
	catch (...) {
		close(handlepair[0]);
		close(handlepair[1]);
		abort_recv_socket = -1;
		throw;
	}
		
	close(handlepair[0]);
	close(handlepair[1]);
	abort_recv_socket = -1;

	return retval;
}

//============================  send_abortable  ==============================
// Emulates a recv. Waits for data on either the TCP socket or the
// abort_send_socket.
//----------------------------------------------------------------------------
int detail::sock_wrapper::send_abortable(const void *buf, std::size_t len, int flags)
{
	std::size_t count = 0;

	// We must allow aborting
	int handlepair[2];

	if (socketpair(PF_UNIX, SOCK_DGRAM, 0, handlepair))
		throw sock_error_ex("Cannot create abort socket pair");

	abort_send_socket = handlepair[0];

	fd_set writeset;
	fd_set readset;

	FD_ZERO(&writeset);
	FD_ZERO(&readset);
	FD_SET(sockhand, &writeset);
	FD_SET(handlepair[1], &readset);

	int maxhandle = std::max(sockhand, handlepair[1]) + 1;

	try {
		while (count < len) {
			if (select(maxhandle, &readset, &writeset, 0, 0) > 0) {
				if (FD_ISSET(sockhand, &writeset)) { /* Is TCP socket writable? */
					int in_count = ::send(sockhand, (char*)buf, len, flags);
					if (in_count<0)
						throw sock_error_ex("Cannot send to socket");
					count += in_count;
				}
				else if (FD_ISSET(handlepair[1], &readset)) { /* Is abort socket readable? */
					char ch;
					::recv(handlepair[1], &ch, sizeof(char), 0);
					// We ignore what we read. It must be an abort command
					break;
				}
			}
			else
				throw sock_error_ex("Cannot select input");
		}
	}
	catch (...) {
		close(handlepair[0]);
		close(handlepair[1]);
		abort_send_socket = -1;
		throw;
	}
	
	close(handlepair[0]);
	close(handlepair[1]);
	abort_send_socket = -1;

	return count;
}
#endif


int detail::sock_wrapper::recv_format(void *buf, std::size_t len, int flags)
{
	test_open();

	return inf
			? inf->input(buf,len,flags)
			: recv_noformat(buf,len,flags);
}

int detail::sock_wrapper::recv_noformat(void *buf, std::size_t len, int flags)
{
	int val = recv_abortable(buf, len, flags);
#if 0
	int val = ::recv(sockhand, (char*)buf, len, flags);

	if (val<0)
		throw sock_error_ex("Cannot receive from socket");
#endif

	return val==0 ? -1 : val;
}

int detail::sock_wrapper::send_format(const void *buf, std::size_t len, int flags)
{
	test_open();

	return outf
			? outf->output(buf,len,flags)
			: send_noformat(buf,len,flags);
}

int detail::sock_wrapper::send_noformat(const void *buf, std::size_t len, int flags)
{
	int val = send_abortable(buf, len, flags);
#if 0
	int val = ::send(sockhand, (char*)buf, len, flags);

	if (val<0)
		throw sock_error_ex("Cannot send to socket");
#endif

	return val;
}

void detail::sock_wrapper::set_output_formatter(dti::sockets::output_formatter *outform)
{
	delete outf;

	outf = outform;
	outf->sw = this;
}

void detail::sock_wrapper::set_input_formatter(dti::sockets::input_formatter *inform)
{
	delete inf;

	inf = inform;
	inf->sw = this;
}

#ifdef _WIN32
void detail::sock_wrapper::abort()
{
	if (abort_send_event)
		SetEvent(abort_send_event);

	if (abort_recv_event)
		SetEvent(abort_recv_event);
}
#endif

#ifdef __GNUC__
void detail::sock_wrapper::abort()
{
	const char abort_command = 1;

	if (abort_recv_socket!=-1)
		::send(abort_recv_socket, &abort_command, sizeof(char), 0);

	if (abort_send_socket!=-1)
		::send(abort_send_socket, &abort_command, sizeof(char), 0);
}
#endif


//***************************  TCP_BASE_SOCKET  ******************************
// Base class for communication sockets and listening sockets
//----------------------------------------------------------------------------

tcp_base_socket::tcp_base_socket()
{
	sw = new detail::sock_wrapper();
}

tcp_base_socket::tcp_base_socket(socket_handle s)
{
	int optval;
	int optlen = sizeof optval;

  if (getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&optval, (socklen_t *) &optlen))
		throw sock_error_ex("Cannot get socket options");
	if (optval!=SOCK_STREAM)
		throw sock_error_ex("Socket type is not SOCK_STREAM");

	// Is there a way to test the domain of the socket, i.e. that it is indeed
	// a PF_INET socket?

	sw = new detail::sock_wrapper(s);
}

tcp_base_socket::tcp_base_socket(const tcp_base_socket& tp)
	: sw(tp.sw)
{
	// Increment the reference count on the socket handle
	sw->inc_refcount();
}

tcp_base_socket::~tcp_base_socket()
{
	if (sw->dec_refcount()==0)
		delete sw;
}

tcp_base_socket& tcp_base_socket::operator=(const tcp_base_socket& tp)
{
	// Increment the reference count on the socket handle for the right hand side
	tp.sw->inc_refcount();

	// Decrement the reference count on the socket handle for the left hand side
	if (this->sw->dec_refcount()==0)
		delete this->sw;

	sw = tp.sw;

	return *this;
}

void tcp_base_socket::open()
{
	sw->open();
}

void tcp_base_socket::close()
{
	sw->test_open();
	sw->close_if_open();
}

bool tcp_base_socket::is_open() const
{
	return sw->is_open();
}

socket_handle tcp_base_socket::get_socket_handle() const
{
	sw->test_open();
	return sw->get_socket_handle();
}

endpoint tcp_base_socket::my_endpoint() const
{
	sw->test_open();

	sockaddr_in si;
	int namelen = sizeof si;

  if (::getsockname(get_socket_handle(), (sockaddr*)&si, (socklen_t*)&namelen))
		throw sock_error_ex("Cannot get this endpoint");

	return endpoint(ipaddr(si.sin_addr.s_addr), ntohs(si.sin_port));
}

void tcp_base_socket::abort()
{
	sw->abort();
}


//******************************  TCP_SOCKET  ********************************
// Class for communicating sockets
//----------------------------------------------------------------------------

tcp_socket::tcp_socket()
{
}

tcp_socket::tcp_socket(socket_handle s)
	: tcp_base_socket(s)
{
}

tcp_socket::tcp_socket(const tcp_socket& tp)
	: tcp_base_socket(tp)
{
}


bool tcp_socket::is_connected() const
{
	// TODO: Is this the easiest way to do this?

	if (!is_open())
		return false;

	sockaddr_in si;
	int namelen = sizeof si;

    return ::getpeername(get_socket_handle(), (sockaddr*)&si, (socklen_t*)&namelen) == 0;
}

endpoint tcp_socket::peer() const
{
	sockaddr_in si;
	int namelen = sizeof si;

  if (::getpeername(get_socket_handle(), (sockaddr*)&si, (socklen_t*)&namelen))
		throw sock_error_ex("Cannot get the peer endpoint");

	return endpoint(ipaddr(si.sin_addr.s_addr), ntohs(si.sin_port));
}

int tcp_socket::fionread() const
{
	unsigned long count;

#ifdef _WIN32
	int val = ioctlsocket(get_socket_handle(), FIONREAD, &count);
#else
	int val = ioctl(get_socket_handle(), FIONREAD, &count);
#endif
	if (val<0)
		throw sock_error_ex("Cannot get socket data count");

	return count;
}


int tcp_socket::recv(void *buf, std::size_t len, int flags)
{
	return sw->recv_format(buf, len, flags);
}

int tcp_socket::recv(string& buf, std::size_t maxlen, int flags)
{
	char *mybuf = new char[maxlen];

	int len;
	try {
		len = sw->recv_format(mybuf, maxlen, flags);
	}
	catch (...) {
		delete[] mybuf;
		throw;
	}

	if (len>=0)
		buf = string(mybuf, len);
	delete[] mybuf;
	return len;
}

int tcp_socket::send(const void *buf, std::size_t len, int flags)
{
	return sw->send_format(buf, len, flags);
}

int tcp_socket::send(const string& buf, int flags)
{
	return sw->send_format(buf.c_str(), buf.length(), flags);
}

void tcp_socket::set_output_formatter(dti::sockets::output_formatter *outform)
{
	sw->set_output_formatter(outform);
}

void tcp_socket::set_input_formatter(dti::sockets::input_formatter *inform)
{
	sw->set_input_formatter(inform);
}


//**************************  TCP_CLIENT_SOCKET  *****************************
// Client socket class
//----------------------------------------------------------------------------

tcp_client_socket::tcp_client_socket()
{
}

tcp_client_socket::tcp_client_socket(const endpoint& n)
{
	connect(n);
}


void tcp_client_socket::connect(const endpoint& n)
{
	if (!is_open())
		open();

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(n.get_port());
	sa.sin_addr.s_addr = n.get_ipaddr().to_uint();

	if (::connect(get_socket_handle(), (struct sockaddr*)&sa, sizeof(sa)) != 0)
		throw sock_error_ex("Cannot connect");
}

//*****************************  TCP_LISTENER  *******************************
// Listening socket class
//----------------------------------------------------------------------------

tcp_listener::tcp_listener()
	: isbound(false),
	  islistening(false)
{
}

tcp_listener::tcp_listener(const endpoint& n)
	: ep(n),
	  isbound(false),
	  islistening(false)
{
}

void tcp_listener::close()
{
	isbound = false;
	islistening = false;
	tcp_base_socket::close();
}

void tcp_listener::bind()
{
	if (!is_open())
		open();

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(ep.get_port());
	sa.sin_addr.s_addr = ep.get_ipaddr().to_uint();
	if (::bind(get_socket_handle(), (struct sockaddr*)&sa, sizeof(sa)))
		throw sock_error_ex("Cannot bind socket");

	isbound = true;
}

void tcp_listener::bind(const endpoint& n)
{
	ep = n;
	bind();
}

void tcp_listener::listen(int backlog)
{
	if (!isbound)
		bind();

	if (::listen(get_socket_handle(), backlog))
		throw sock_error_ex("Socket cannot listen");

	islistening = true;
}

tcp_socket tcp_listener::accept()
{
	if (!islistening)
		listen();

  // Modified by PA
	/* socket_handle */ int s = ::accept(get_socket_handle(), 0, 0);

	if (s<0)
		throw sock_error_ex("Socket cannot accept connection");

	return tcp_socket(s);
}

//***************************  OUTPUT_FORMATTER  *****************************
// Abstract base class for output formatters
//----------------------------------------------------------------------------

output_formatter::output_formatter()
	: sw(0)
{
}

int output_formatter::formatterout(const void *buf, std::size_t len, int flags)
{
	return sw->send_noformat(buf,len,flags);
}


//***************************  INPUT_FORMATTER  ******************************
// Abstract base class for input formatters
//----------------------------------------------------------------------------

input_formatter::input_formatter()
	: sw(0)
{
}

int input_formatter::formatterin(void *buf, std::size_t len, int flags)
{
	return sw->recv_noformat(buf,len,flags);
}


//************************  LINE_OUTPUT_FORMATTER  ***************************
// Output formatter for lines of text
//----------------------------------------------------------------------------

int line_output_formatter::output(const void *buf, std::size_t len, int flags)
{
	int val;
	char *newbuf = new char[len+2];

	memcpy(newbuf, buf, len);
	memcpy(newbuf+len, "\r\n", 2);

	try {
		val = formatterout(newbuf,len+2,flags);
	}
	catch (...) {
		delete[] newbuf;
		throw;
	}

	delete[] newbuf;
	return val-2;
}

//*************************  LINE_INPUT_FORMATTER  ***************************
// Input formatter for lines of text
//----------------------------------------------------------------------------

int line_input_formatter::input(void *buf, std::size_t len, int flags)
{
	char mybuf[1024];
	string::size_type ix;

	while (true) {
		ix = tmp.find('\n');
		if (ix!=string::npos)
			break;

		int i = formatterin(mybuf, sizeof mybuf, flags);

		if (i==-1)
			return -1;

		tmp += string(mybuf,i);
	}

	const char *ib = tmp.c_str();
	char *ob = (char*)buf;
	int count = 0;

	for (string::size_type i=0; i<ix; ++i)
		if (*ib!='\r') {
			*ob++ = *ib++;
			count++;
		}

	tmp.erase(0, ix+1);

	return count;
}

//***********************  LENGTH_OUTPUT_FORMATTER  **************************
// Output formatter which precedes each record with a length
//----------------------------------------------------------------------------

int length_output_formatter::output(const void *buf, std::size_t len, int flags)
{
	int val;
	char *newbuf = new char[len+sizeof(int)];
	memcpy(newbuf+sizeof(int), buf, len);
	*reinterpret_cast<int*>(newbuf) = htonl(len);

	try {
		val = formatterout((const void*)newbuf,len+sizeof(int),flags);
	}
	catch (...) {
		delete[] newbuf;
		throw;
	}

	delete[] newbuf;
	return val-sizeof(int);
}

//************************  LENGTH_INPUT_FORMATTER  **************************
// Input formatter which assumes each record is preceded by a length
//----------------------------------------------------------------------------

int length_input_formatter::input(void *buf, std::size_t len, int flags)
{
	char mybuf[1024];

	while (tmp.length()<sizeof(int)) {
		int i = formatterin(mybuf, sizeof mybuf, flags);
		if (i==-1)
			return -1;

		tmp += string(mybuf,i);
	}

	int expect = ntohl(*reinterpret_cast<const int*>(tmp.c_str()));

	while (tmp.length()<sizeof(int)+expect) {
		int i = formatterin(mybuf, sizeof mybuf, flags);
		if (i==-1)
			return -1;

		tmp += string(mybuf,i);
	}

	memcpy(buf, tmp.c_str()+sizeof(int), expect);
	tmp.erase(0, expect+sizeof(int));

	return expect;
}

//************************  FIXED_INPUT_FORMATTER  ***************************
// Input formatter for fixed-length records
//----------------------------------------------------------------------------

int fixed_input_formatter::input(void *buf, std::size_t len, int flags)
{
	char mybuf[1024];

	while (tmp.length()<len) {
		int i = formatterin(mybuf, sizeof mybuf, flags);
		if (i==-1)
			return -1;

		tmp += string(mybuf,i);
	}
	memcpy(buf, tmp.c_str(), len);
	tmp.erase(0, len);

	return len;
}
