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

#include "mem65x64.hpp"

mem65x64::Addr  mem65x64::memMask;
mem65x64::Addr  mem65x64::ramSize;

mem65x64::Byte   *mem65x64::pRAM;
const mem65x64::Byte *mem65x64::pROM;

//==============================================================================

// Never used.
mem65x64::mem65x64()
{ }

// Never used.
mem65x64::~mem65x64()
{ }

// Sets up the memory areas using a dynamically allocated array
void mem65x64::setMemory(Addr memMask, Addr ramSize, const Byte *pROM)
{
    setMemory(memMask, ramSize, new Byte[ramSize], pROM);
}

// Sets up the memory area using pre-allocated array
void mem65x64::setMemory(Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM)
{
    mem65x64::memMask = memMask;
    mem65x64::ramSize = ramSize;
    mem65x64::pRAM = pRAM;
    mem65x64::pROM = pROM;
}
