#ifndef __NSEXPORT_H#define __NSEXPORT_H
#if defined(_DANSNSBBDLL)	#define _NSBBCLASSE __export
#else
	#define _NSBBCLASSE __import
#endif

#endif         // __NSEXPORT_H
