#ifndef XBVM_LOG_H
#define XBVM_LOG_H

#include <cstdio>

#define DPRINT(format,...) \
	do { \
		std::printf( format, ##__VA_ARGS__ ); \
		std::printf( "\n" ); \
	} while(0)

#define EPRINT(format,...) \
	do { \
		std::fprintf( stderr, "ERROR:  " ); \
		std::fprintf( stderr, format, ##__VA_ARGS__ ); \
		std::fprintf( stderr, "\n" ); \
	} while(0)

#endif /*XBVM_LOG_H*/

