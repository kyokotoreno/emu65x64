//==============================================================================
//                         ____  _____       ____    ___
//                        / ___||  ___|     / ___|  /   |
//    ___ _ __ ___  _   _/ /___ |___ \__  _/ /___  / /| |
//   / _ \ '_ ` _ \| | | | ___ \    \ \ \/ / ___ \/ /_| |
//  |  __/ | | | | | |_| | \_/ |/\__/ />  <| \_/ |\___  |
//   \___|_| |_| |_|\__,_\_____/\____//_/\_\_____/    |_/
//
// A Portable C++ NOZOTECH 65x64 Emulator
//------------------------------------------------------------------------------
// Copyright (C),2024 KyokoToreno
// Based on the work of: (C),2016 Andrew John Jacobs
// All rights reserved.
//
// This work is made available under the terms of the Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
// following URL to see the details.
//
// http://creativecommons.org/licenses/by-nc-sa/4.0/
//------------------------------------------------------------------------------

#ifndef NOZO65X64_H
#define NOZO65X64_H

// The nozo65x64 class defines common types for 8-, 16-, 32- and 64-bit data values and
// a set of common functions for manipulating them.

class nozo65x64
{
public:
    // Common types for memory and register sizes
    typedef unsigned char       Bit;
    typedef unsigned char       Byte;
    typedef unsigned short      Word;
    typedef unsigned long       Dword;
    typedef unsigned long long  Qword, Addr;

    // Convert a value to a hex string.
    static char *toHex(unsigned long long value, unsigned int digits);

    // Return the low byte of a word
    inline static Byte lo_b(Word value)
    {
        return ((Byte) value);
    }

    // Return the high byte of a word
    inline static Byte hi_b(Word value)
    {
        return (lo_b(value >> 8));
    }

    // Return the low word of a dword
    inline static Word lo_w(Dword value)
    {
        return ((Word) value);
    }

    // Return the high word of a dword
    inline static Word hi_w(Dword value)
    {
        return (lo_w(value >> 16));
    }

    // Return the low dword of a qword
    inline static Dword lo_d(Qword value)
    {
        return ((Dword) value);
    }

    // Return the high dword of a qword
    inline static Dword hi_d(Qword value)
    {
        return (lo_d(value >> 32));
    }

    // Combine two bytes into a word
    inline static Word join_w(Byte l, Byte h)
    {
        return (l | (h << 8));
    }

    // Combine two words into a dword
    inline static Dword join_d(Word l, Word h)
    {
        return (l | (h << 16));
    }

    // Combine two dwords into a qword
    inline static Qword join_q(Dword l, Dword h)
    {
        return (l | ((Qword)h << 32));
    }

    // Swap the high and low bytes of a word
    inline static Word swap_w(Word value)
    {
        return ((value >> 8) | (value << 8));
    }

    // Swap the high and low words of a dword
    inline static Dword swap_d(Dword value)
    {
        return ((value >> 16) | (value << 16));
    }

    // Swap the high and low dwords of a qword
    inline static Qword swap_q(Qword value)
    {
        return ((value >> 32) | (value << 32));
    }

protected:
    nozo65x64();
    ~nozo65x64();
};
#endif
