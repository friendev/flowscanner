#ifndef __FlowScanner_H
#define __FlowScanner_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#include <avr/pgmspace.h>
#else
	#if defined(WIN32) || defined(UNIX)

		#include <inttypes.h>
		#include <iostream>

		#define PSTR(x) x
		#define pgm_read_byte(str) (*(str))
		#define PGM_P const char*
		#define PROGMEM

		#define min(x,y) ((x) < (y)? (x) : (y))

	#endif
#endif

#include <stdarg.h>


//defines a new FlowPattern object with its pattern string stored in PROGMEM
#define DEFINE_FLOWPATTERN(name, pattern) const char name##string[] PROGMEM = pattern;FlowPattern name ( name##string );


class FlowScanner;

class FlowPattern
{
	friend class FlowScanner;

	PGM_P pattern;
	
public:
	bool signaled;
	FlowPattern(PGM_P scanPattern);

};

class FlowScanner 
{
	static const uint8_t DEFAULT_DECIMAL_WIDTH = 80;
	static const uint8_t DEFAULT_WIDTH_MARKER = 0xFF;

	FlowPattern* pattern;
	PGM_P p;
	char t;
	char specifier;

	uint8_t width;
	uint8_t captureCount;

	union
	{
		struct
		{
			int32_t value;
			uint8_t length;
			int8_t sign;
		};
		struct
		{
			uint8_t* charPtr;
			PGM_P scanset;
			bool negatedScanset;
		};
	};


	bool enableCapture;


public:



	void setPattern(FlowPattern& flowPattern);


	bool scan(uint8_t c, ...);
	bool scan(uint8_t** buffer, uint16_t* length, ...);

	void reset();
	bool isSignaled();


private :
	void setPos(va_list &args, va_list init, uint8_t pos);
	bool isInScanset(uint8_t c);
	bool input(uint8_t c, va_list& args);
	
	bool init_d(uint8_t c);
	void init_c(va_list &args, va_list init,uint8_t c);
	void init_scanset(uint8_t c);

	void captureInteger(va_list &args);
	char nextChar();
};


#endif