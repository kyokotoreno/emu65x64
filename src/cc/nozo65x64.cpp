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

#include "nozo65x64.hpp"

// Never used.
nozo65x64::nozo65x64()
{ }

// Never used.
nozo65x64::~nozo65x64()
{ }

// Convert a value to a hex string
char *nozo65x64::toHex(unsigned long long value, unsigned int digits)
{
    static char buffer[32];
    unsigned int offset = sizeof(buffer);

    buffer[--offset] = 0;
    while (digits-- > 0) {
        buffer[--offset] = "0123456789ABCDEF"[value & 0xf];
        value >>= 4;
    }
    return (&(buffer[offset]));
}
