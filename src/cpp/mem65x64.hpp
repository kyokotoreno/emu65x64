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

#ifndef MEM65X64_H
#define MEM65X64_H

#include "nozo65x64.hpp"

// The mem65x64 class defines a set of standard methods for defining and accessing
// the emulated memory area.

class mem65x64 :
    public nozo65x64
{
public:
    // Define the memory areas and sizes
    static void setMemory (Addr memMask, Addr ramSize, const Byte *pROM);
    static void setMemory (Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM);

    // Fetch a byte from memory
    inline static Byte getByte(Addr ea)
    {
        if ((ea &= memMask) < ramSize)
            return (pRAM[ea]);

        return (pROM[ea - ramSize]);
    }

    // Fetch a word from memory
    inline static Word getWord(Addr ea)
    {
        return (join_w(getByte(ea + 0), getByte(ea + 1)));
    }

    // Fetch a dword from memory
    inline static Dword getDword(Addr ea)
    {
        return (join_d(getWord(ea + 0), getWord(ea + 2)));
    }

    // Fetch a qword from memory
    inline static Qword getQword(Addr ea)
    {
        return (join_q(getDword(ea + 0), getDword(ea + 4)));
    }

    // Fetch a long address from memory
    inline static Addr getAddr(Addr ea)
    {
        return (Addr)getQword(ea);
    }

    // Write a byte to memory
    inline static void setByte(Addr ea, Byte data)
    {
        if ((ea &= memMask) < ramSize)
            pRAM[ea] = data;
    }

    // Write a word to memory
    inline static void setWord(Addr ea, Word data)
    {
        setByte(ea + 0, lo_b(data));
        setByte(ea + 1, hi_b(data));
    }

    // Write a dword to memory
    inline static void setDword(Addr ea, Dword data)
    {
        setWord(ea + 0, lo_w(data));
        setWord(ea + 2, hi_w(data));
    }

    // Write a qword to memory
    inline static void setQword(Addr ea, Qword data)
    {
        setDword(ea + 0, lo_d(data));
        setDword(ea + 4, hi_d(data));
    }

protected:
    mem65x64();
    ~mem65x64();

private:
    static Addr         memMask;        // The address mask pattern
    static Addr         ramSize;        // The amount of RAM

    static Byte       *pRAM;            // Base of RAM memory array
    static const Byte *pROM;            // Base of ROM memory array
};
#endif
