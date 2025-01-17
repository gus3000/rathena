// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h> // FILE*
#include <string>
#include <memory>

#include "cbasetypes.hpp"

// generate a hex dump of the first 'length' bytes of 'buffer'
void WriteDump(FILE* fp, const void* buffer, size_t length);
void ShowDump(const void* buffer, size_t length);

int check_filepath(const char* filepath);
void findfile(const char *p, const char *pat, void (func)(const char*));
bool exists(const char* filename);

/// Caps values to min/max
#define cap_value(a, min, max) (((a) >= (max)) ? (max) : ((a) <= (min)) ? (min) : (a))

/// Apply rate for val, divided by 100)
#define apply_rate(val, rate) (((rate) == 100) ? (val) : ((val) > 100000) ? ((val) / 100 * (rate)) : ((val) * (rate) / 100))

/// Apply rate for val, divided by per
#define apply_rate2(val, rate, per) (((rate) == (per)) ? (val) : ((val) > 100000) ? ((val) / (per) * (rate)) : ((val) * (rate) / (per)))

/// calculates the value of A / B, in percent (rounded down)
unsigned int get_percentage(const unsigned int A, const unsigned int B);
uint32 get_percentage_exp(const uint64 a, const uint64 b);

//////////////////////////////////////////////////////////////////////////
// byte word dword access [Shinomori]
//////////////////////////////////////////////////////////////////////////

extern uint8 GetByte(uint32 val, int idx);
extern uint16 GetWord(uint32 val, int idx);
extern uint16 MakeWord(uint8 byte0, uint8 byte1);
extern uint32 MakeDWord(uint16 word0, uint16 word1);

//////////////////////////////////////////////////////////////////////////
// Big-endian compatibility functions
//////////////////////////////////////////////////////////////////////////
extern int16 MakeShortLE(int16 val);
extern int32 MakeLongLE(int32 val);
extern uint16 GetUShort(const unsigned char* buf);
extern uint32 GetULong(const unsigned char* buf);
extern int32 GetLong(const unsigned char* buf);
extern float GetFloat(const unsigned char* buf);


template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
//    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    if( size_s <= 0 ){ return "Error during formatting." ; }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
#endif /* UTILS_HPP */
