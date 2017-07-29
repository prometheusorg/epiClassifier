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
#include <dti/async.h>
#include <dti/abortable.h>
#include "events.h"

#ifdef _WIN32
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x0500 // We need this in order to use QueueUserWorkItem()
#	endif
#	include <windows.h>
#	include <process.h>
#endif
#ifdef __GNUC__
#	include <pthread.h>
#endif

using namespace dti::async;
using namespace dti::async::detail;

#if defined(UNICODE) || defined(_UNICODE)
	typedef wchar_t chichar_t;
#	define CHIT(txt) L##txt
#else
	typedef char chichar_t;
#	define CHIT(txt) txt
#endif


//*********************************  DOIO  ***********************************
// The doio() function is called asynchronously. It performs all
// the pending I/O operations on a given I/O stream, passed as the
// parameter to this function.
//----------------------------------------------------------------------------

#ifdef _WIN32
static unsigned __stdcall doio(void* lpParameter)
#endif
#ifdef __GNUC__
static void* doio(void* lpParameter)
#endif
{
	half_io *hiop = (half_io*)lpParameter;
	hiop->hasexcep=false;
	try {
		for (std::vector<iotarget*>::iterator it=hiop->vec.begin();
			 				it!=hiop->vec.end() && !hiop->do_abort; ++it)
			(*it)->invoke();

		hiop->abort_reserve.enter();
		hiop->do_abort = false;
		hiop->may_abort = false;
		hiop->abort_reserve.leave();
	}
	catch (std::runtime_error& e) {
		hiop->hasexcep = true;
		hiop->excepstr = e.what();
	}
	catch (std::exception&) {
		hiop->hasexcep = true;
		hiop->excepstr = CHIT("Unknown exception");
	}
	set_event(hiop->event_id);
#ifdef _WIN32
	_endthread();
#endif
	return 0;
}

//*******************************  HALF_IO  **********************************
//
//
//
//----------------------------------------------------------------------------

half_io::half_io()
	: is_running(false), threadid(0)
{
	event_id = create_event(false);
	if (!event_id)
		throw std::runtime_error("Cannot create event handle");
	may_abort = false;
	do_abort = false;
}

half_io::~half_io()
{
	delete_event(event_id);
	for (std::vector<iotarget*>::iterator it=vec.begin(); it!=vec.end(); ++it)
		delete *it;
}

void half_io::cleanup()
{
	for (std::vector<iotarget*>::iterator it=vec.begin(); it!=vec.end(); ++it)
		delete *it;
	vec.clear();

#ifdef _WIN32
    WaitForSingleObject((HANDLE)threadid, INFINITE);
    CloseHandle((HANDLE)threadid);
#endif
#ifdef __GNUC__
	pthread_join(threadid, 0);
#endif

	threadid = 0;

	clear_running();
}

void half_io::test_running()
{
	if (is_running)
		throw std::runtime_error("I/O stream is busy");
}



event half_io::iostart()
{
	if (!vec.empty()) {
		if (!is_running) {
			set_running();
			may_abort = true;

#ifdef _WIN32			
			threadid = _beginthreadex(0, 0, doio, this, 0, 0);
			if (!threadid)
				throw std::runtime_error("Cannot start asynchronous handler");
#endif
#ifdef __GNUC__
			if (pthread_create(&threadid, 0, doio, this))
				throw std::runtime_error("Cannot start asynchronous handler");
#endif
		}
		return event_id;
	}
	else
		return 0;
}

//**************************  SUPER_IO_OPERATION  ****************************
// Base class for all io_operation classes
//----------------------------------------------------------------------------

super_io_operation::super_io_operation()
{
	hio[input].container = this;
	hio[input].dir = input;
	hio[output].container = this;
	hio[output].dir = output;
}

//*****************************  ASYNC_ERROR  ********************************
// Exception thrown by the asynchronous mechanism
//----------------------------------------------------------------------------

async_error::async_error()
	: std::runtime_error(""), bad_ios(0)
{
}

async_error::async_error(const std::string& arg, std::ios_base *badios)
	: std::runtime_error(arg), bad_ios(badios)
{
}

std::ios_base *async_error::whatios() const
{
	return bad_ios;
}

//*******************************  ASTREAM  **********************************
// Objects of the astream class identify sets of I/O operations that must be
// performed concurrently. Consider this sample code:
//----------------------------------------------------------------------------

void astream::start()
{
	if (!iop.empty())
		for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it) {
			(*it)->hio[input].iostart();
			(*it)->hio[output].iostart();
		}
}

std::ios_base* astream::wait(long timeout_ms, wait_type wait_for, io_direction *dir)
{
	std::ios_base *completed_str;
	bool first = true;

	hasexcep = false;

	while (first || wait_for==wait_for_all) {
		first = false;

		std::vector<detail::super_io_operation*>::size_type iop_count = iop.size();
		if (iop_count==0)
			return 0;			// We have no io_operations to handle


		// hioop[n] describes the ownership of handles[n]
		// We need at most iop_count*2 handles
		typedef half_io* hioptr;
		half_io **hioop = new hioptr[iop_count*2];
		event *handles = new event[iop_count*2];

		int handle_counter=0;

		// For each I/O operation we check if it has pending input or output
		// requests, and if so, we note its handle
		for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it) {
			for (io_direction iod=input; iod<=output; iod=static_cast<io_direction>(iod+1)) {
				handles[handle_counter] = (*it)->hio[iod].iostart();
				if (handles[handle_counter]) {
					// We have an I/O request
					hioop[handle_counter] = &(*it)->hio[iod];
					++handle_counter;
				}
			}
		}

		if (handle_counter==0) {
			for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it)
				delete *it;
			iop.clear();
			return 0;
		}

		int i = wait_for_multiple_events(handle_counter, handles,
									   wait_for==wait_for_all, timeout_ms==0 ? -1 : timeout_ms);

		if (i==-1) {
			// Timeout
			delete[] handles;
			delete[] hioop;
			return 0;
		}

		reset_event(handles[i]);
		delete[] handles;
		completed_str = hioop[i]->container->get_mystr();
		if (hioop[i]->hasexcep) {
			hasexcep = true;
			excep = async_error(hioop[i]->excepstr, completed_str);
		}

		dti::abortable::ios_abortable *abortable =
			dynamic_cast<dti::abortable::ios_abortable*>(completed_str);
		if (abortable)
			abortable->clear_abort();

		hioop[i]->cleanup();

		if (wait_for==wait_for_one && dir!=0)
			*dir = hioop[i]->dir;

		delete[] hioop;
	}

	if (hasexcep)
		throw excep;

	return completed_str;
}

std::ios_base* astream::poll(io_direction *dir)
{
	std::ios_base *completed_str;

	hasexcep = false;

	std::vector<detail::super_io_operation*>::size_type iop_count = iop.size();
	if (iop_count==0)
		return 0;				// We have no io_operations to handle


	// hioop[n] describes the ownership of handles[n]
	// We need at most iop_count*2 handles
	typedef half_io* hioptr;
	half_io **hioop = new hioptr[iop_count*2];
	event *handles = new event[iop_count*2];

	int handle_counter=0;

	// For each I/O operation we check if it has pending input or output
	// requests, and if so, we note its handle
	for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it) {
		for (io_direction iod=input; iod<=output; iod=static_cast<io_direction>(iod+1)) {
			handles[handle_counter] = (*it)->hio[iod].iostart();
			if (handles[handle_counter]) {
				// We have an I/O request
				hioop[handle_counter] = &(*it)->hio[iod];
				++handle_counter;
			}
		}
	}

	if (handle_counter==0) {
		for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it)
			delete *it;
		iop.clear();
		return 0;
	}

	int i = wait_for_multiple_events(handle_counter, handles, false, 0);

	if (i==-1) {
		// "Timeout" - which means that the poll failed
		delete[] handles;
		delete[] hioop;
		return 0;
	}

	reset_event(handles[i]);
	delete[] handles;
	completed_str = hioop[i]->container->get_mystr();
	if (hioop[i]->hasexcep) {
		hasexcep = true;
		excep = async_error(hioop[i]->excepstr, completed_str);
	}

	dti::abortable::ios_abortable *abortable =
		dynamic_cast<dti::abortable::ios_abortable*>(completed_str);
	if (abortable)
		abortable->clear_abort();


	hioop[i]->cleanup();

	if (dir)
		*dir = hioop[i]->dir;

	delete[] hioop;

	if (hasexcep)
		throw excep;

	return completed_str;
}

int astream::pendio()
{
	int counter=0;

	// For each I/O operation we check if it has pending input or output
	// requests, and if so, we increment the counter
	
	for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it)
		for (io_direction iod=input; iod<=output; iod=static_cast<io_direction>(iod+1))
			if (!(*it)->hio[iod].vec.empty())
				++counter;

	return counter;
}

void astream::abort()
{
	std::ios_base *non_abortable = 0;

	// For each I/O operation we check if it has pending input or output
	// requests, and if so, we abort the associated operation if we can
	
	for (std::vector<detail::super_io_operation*>::iterator it=iop.begin(); it!=iop.end(); ++it)
		for (io_direction iod=input; iod<=output; iod=static_cast<io_direction>(iod+1)) {
			half_io *hiop = &(*it)->hio[iod];

			if (!hiop->may_abort) /* Quick sample without reserving abort_reserve */
				continue;

			hiop->abort_reserve.enter();

			if (hiop->may_abort) {
				hiop->do_abort = true;

				std::ios_base *abortme = hiop->container->get_mystr();
				dti::abortable::ios_abortable *abortable =
					dynamic_cast<dti::abortable::ios_abortable*>(abortme);

				if (abortable) {
					try {
						abortable->abort();
					}
					catch (...) {
						hiop->abort_reserve.leave();
						throw;
					}
				}
				else
					non_abortable = abortme;
			}

			hiop->abort_reserve.leave();
		}

	if (non_abortable)
		throw async_error("Operation cannot be aborted", non_abortable);
}
