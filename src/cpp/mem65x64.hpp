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

extern "C" {
    // Rust ffi memory access functions

    unsigned char read_byte(unsigned long long addr);
    unsigned short read_word(unsigned long long addr);
    unsigned long read_dword(unsigned long long addr);
    unsigned long long read_qword(unsigned long long addr);

    void write_byte(unsigned long long addr, unsigned char data);
    void write_word(unsigned long long addr, unsigned short data);
    void write_dword(unsigned long long addr, unsigned long data);
    void write_qword(unsigned long long addr, unsigned long long data);
}

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
        return (Byte)read_byte((unsigned long long)ea);
    }

    // Fetch a word from memory
    inline static Word getWord(Addr ea)
    {
        return (Word)read_word((unsigned long long)ea);
    }

    // Fetch a dword from memory
    inline static Dword getDword(Addr ea)
    {
        return (Dword)read_dword((unsigned long long)ea);
    }

    // Fetch a qword from memory
    inline static Qword getQword(Addr ea)
    {
        return (Qword)read_qword((unsigned long long)ea);
    }

    // Fetch a long address from memory
    inline static Addr getAddr(Addr ea)
    {
        return (Addr)getQword(ea);
    }

    // Write a byte to memory
    inline static void setByte(Addr ea, Byte data)
    {
        write_byte((unsigned long long)ea, (unsigned char)data);
    }

    // Write a word to memory
    inline static void setWord(Addr ea, Word data)
    {
        write_word((unsigned long long)ea, (unsigned short)data);
    }

    // Write a dword to memory
    inline static void setDword(Addr ea, Dword data)
    {
        write_dword((unsigned long long)ea, (unsigned long)data);
    }

    // Write a qword to memory
    inline static void setQword(Addr ea, Qword data)
    {
        write_qword((unsigned long long)ea, (unsigned long long)data);
    }

    // Fallbacks that use pRAM and pROM directly

    // Fetch a byte from memory
    inline static Byte getByteF(Addr ea)
    {
        if ((ea &= memMask) < ramSize)
            return (pRAM[ea]);

        return (pROM[ea - ramSize]);
    }

    // Fetch a word from memory
    inline static Word getWordF(Addr ea)
    {
        return (join_w(getByteF(ea + 0), getByteF(ea + 1)));
    }

    // Fetch a dword from memory
    inline static Dword getDwordF(Addr ea)
    {
        return (join_d(getWordF(ea + 0), getWordF(ea + 2)));
    }

    // Fetch a qword from memory
    inline static Qword getQwordF(Addr ea)
    {
        return (join_q(getDwordF(ea + 0), getDwordF(ea + 4)));
    }

    // Write a byte to memory
    inline static void setByteF(Addr ea, Byte data)
    {
        if ((ea &= memMask) < ramSize)
            pRAM[ea] = data;
    }

    // Write a word to memory
    inline static void setWordF(Addr ea, Word data)
    {
        setByteF(ea + 0, lo_b(data));
        setByteF(ea + 1, hi_b(data));
    }

    // Write a dword to memory
    inline static void setDwordF(Addr ea, Dword data)
    {
        setWordF(ea + 0, lo_w(data));
        setWordF(ea + 2, hi_w(data));
    }

    // Write a qword to memory
    inline static void setQwordF(Addr ea, Qword data)
    {
        setDwordF(ea + 0, lo_d(data));
        setDwordF(ea + 4, hi_d(data));
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

extern "C" {
    // Internal fallbacks

    extern unsigned char mem65x64_getByteF(unsigned long long addr);
    extern unsigned short mem65x64_getWordF(unsigned long long addr);
    extern unsigned long mem65x64_getDwordF(unsigned long long addr);
    extern unsigned long long mem65x64_getQwordF(unsigned long long addr);

    extern void mem65x64_setByteF(unsigned long long addr, unsigned char data);
    extern void mem65x64_setWordF(unsigned long long addr, unsigned short data);
    extern void mem65x64_setDwordF(unsigned long long addr, unsigned long data);
    extern void mem65x64_setQwordF(unsigned long long addr, unsigned long long data);
}
#endif
