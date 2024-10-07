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
// Thi_bs work is made available under the terms of the Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
// following URL to see the details.
//
// http://creativecommons.org/licenses/by-nc-sa/4.0/
//------------------------------------------------------------------------------

#ifndef EMU65X64_H
#define EMU65X64_H

#include "mem65x64.hpp"

#include <stdlib.h>
#include <iostream>
#include <string>

#if 1
# define TRACE(MNEM)    { if (trace) dump(MNEM, ea); }
# define BYTES(N)       { if (trace) bytes(N); pc += N; }
# define SHOWPC()       { if (trace) show(); }
# define ENDL()         { if (trace) cout << endl; }
#else
# define TRACE(MNEM)
# define BYTES(N)       { pc += N; }
# define SHOWPC()
# define ENDL()
#endif

// Defines the NOZOTECH 65x64 emulator.
class emu65x64 :
    public mem65x64
{
public:
    static void reset(bool trace);
    static void step();

    inline static unsigned long getCycles()
    {
        return (cycles);
    }

    inline static bool isStopped()
    {
        return (stopped);
    }

    static union FLAGS {
        struct {
            Bit             f_c : 1; // Carry
            Bit             f_z : 1; // Zero
            Bit             f_i : 1; // Interrupt
            Bit             f_d : 1; // Decimal
            Bit             f_x : 1; // Index (deprecated)
            Bit             f_m : 1; // Memory (deprecated)
            Bit             f_v : 1; // Overflow
            Bit             f_n : 1; // Negative
        };
        Byte            b;
    }   p;

    static Byte      r; // Ring level
    static Bit      e; // Emulation mode (deprecated)

    /**
     * Register set
     *
     * a - Accumulator A
     * b - Accumulator B
     * c - Accumulator C
     * x - Index Register X
     * y - Index Register Y
     * z - Index Register Z
     * sp - Stack Pointer
     * tp - Task Pointer
     * dp - Direct Page Register
     */
    static union REGS {
        Byte            b;
        Word            w;
        Dword           d;
        Qword           q;
    }   a, b, c, x, y, z, sp, tp, dp;

    static Qword     pc; // Program Counter
    static Byte     pbr, dbr; // Program and Data Bank Registers (deprecated)

    static bool     stopped; // Indicates the emulator has stopped
    static bool     interrupted; // Indicates an interrupt has occurred
    static unsigned long cycles; // Number of cycles executed
    static bool     trace; // Indicates trace mode is enabled

    emu65x64();
    ~emu65x64();

    static void show();
    static void bytes(unsigned int);
    static void dump(const char *, Addr);

    // Push a byte on the stack
    inline static void pushByte(Byte value)
    {
        setByte(sp.w, value);

        if (e)
            --sp.b;
        else
            --sp.w;
    }

    // Push a word on the stack
    inline static void pushWord(Word value)
    {
        pushByte(hi_b(value));
        pushByte(lo_b(value));
    }

    // Push a dword on the stack
    inline static void pushDword(Dword value)
    {
        pushWord(hi_w(value));
        pushWord(lo_w(value));
    }

    // Push a qword on the stack
    inline static void pushQword(Qword value)
    {
        pushDword(hi_d(value));
        pushDword(lo_d(value));
    }

    // Pull a byte from the stack
    inline static Byte pullByte()
    {
        if (e)
            ++sp.b;
        else
            ++sp.w;

        return (getByte(sp.w));
    }

    // Pull a word from the stack
    inline static Word pullWord()
    {
        register Byte   l = pullByte();
        register Byte   h = pullByte();

        return (join_w(l, h));
    }

    // Pull a dword from the stack
    inline static Dword pullDword()
    {
        register Word   l = pullWord();
        register Word   h = pullWord();

        return (join_d(l, h));
    }

    // Pull a qword from the stack
    inline static Qword pullQword()
    {
        register Dword  l = pullDword();
        register Dword  h = pullDword();

        return (join_q(l, h));
    }

private:
    // Absolute - a
    inline static Addr am_absl()
    {
        register Addr   ea = getQword(pc);

        BYTES(8);
        cycles += 2; // TODO: fix cycles
        return (ea);
    }

    // Absolute Indexed X - a,X
    inline static Addr am_absx()
    {
        register Addr   ea = getQword(pc) + x.q;

        BYTES(8);
        cycles += 2; // TODO: fix cycles
        return (ea);
    }

    // Absolute Indexed Y - a,Y
    inline static Addr am_absy()
    {
        register Addr   ea = getQword(pc) + y.q;

        BYTES(8);
        cycles += 2; // TODO: fix cycles
        return (ea);
    }

    // Absolute Indexed Z - a,Z
    inline static Addr am_absz()
    {
        register Addr   ea = getQword(pc) + z.q;

        BYTES(8);
        cycles += 2; // TODO: fix cycles
        return (ea);
    }

    // Absolute Indirect - (a)
    inline static Addr am_absi()
    {
        register Addr ia = getQword(pc);

        BYTES(8);
        cycles += 4; // TODO: fix cycles
        return (getQword(ia));
    }

    // Absolute Indexed Indirect X - (a,X)
    inline static Addr am_abxi()
    {
        register Addr ia = getQword(pc) + x.q;

        BYTES(2);
        cycles += 4; // TODO: fix cycles
        return (getQword(ia));
    }

    // Absolute Indexed Indirect Y - (a,Y)
    inline static Addr am_abyi()
    {
        register Addr ia = getQword(pc) + y.q;

        BYTES(2);
        cycles += 4; // TODO: fix cycles
        return (getQword(ia));
    }

    // Absolute Indexed Indirect Z - (a,Z)
    inline static Addr am_abzi()
    {
        register Addr ia = getQword(pc) + z.q;

        BYTES(2);
        cycles += 4; // TODO: fix cycles
        return (getQword(ia));
    }

    // Absolute Long - >a
    /*
    inline static Addr am_alng()
    {
        Addr ea = getAddr(join_w(pbr, pc));

        BYTES(3);
        cycles += 3; // TODO: fix cycles
        return (ea);
    }

    // Absolute Long Indexed - >a,X
    inline static Addr am_alnx()
    {
        register Addr ea = getAddr(join_w(pbr, pc)) + x.w;

        BYTES(3);
        cycles += 3; // TODO: fix cycles
        return (ea);
    }

    // Absolute Indirect Long - [a]
    inline static Addr am_abil()
    {
        register Addr ia = bank(0) | getWord(join_w(pbr, pc));

        BYTES(2);
        cycles += 5; // TODO: fix cycles
        return (getAddr(ia));
    }
    */

    // Direct Page - d
    inline static Addr am_dpag()
    {
        Dword offset = getDword(pc);

        BYTES(4);
        cycles += 1; // TODO: fix cycles
        return (Qword)(dp.q + offset);
    }

    // Direct Page Indexed X - d,X
    inline static Addr am_dpgx()
    {
        Dword offset = getDword(pc) + x.d;

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 1; // TODO: fix cycles
        return (Qword)(dp.q + offset);
    }

    // Direct Page Indexed Y - d,Y
    inline static Addr am_dpgy()
    {
        Dword offset = getDword(pc) + y.d;

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 1; // TODO: fix cycles
        return (Qword)(dp.q + offset);
    }

    // Direct Page Indexed Z - d,Z
    inline static Addr am_dpgz()
    {
        Dword offset = getDword(pc) + z.d;

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 1; // TODO: fix cycles
        return (Qword)(dp.q + offset);
    }

    // Direct Page Indirect - (d)
    inline static Addr am_dpgi()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (Qword)(getQword(dp.q + offset));
    }

    // Direct Page Indexed Indirect X - (d,x)
    inline static Addr am_dpxi()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (Qword)(dp.q + offset + x.q);
    }

    // Direct Page Indexed Indirect Y - (d,y)
    inline static Addr am_dpyi()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (Qword)(dp.q + offset + y.q);
    }

    // Direct Page Indexed Indirect Z - (d,z)
    inline static Addr am_dpzi()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (Qword)(dp.q + offset + z.q);
    }

    // Direct Page Indirect Indexed X - (d),X
    inline static Addr am_dpix()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (getQword((Qword)(dp.q + offset)) + x.q);
    }

    // Direct Page Indirect Indexed Y - (d),Y
    inline static Addr am_dpiy()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (getQword((Qword)(dp.q + offset)) + y.q);
    }

    // Direct Page Indirect Indexed Z - (d),Z
    inline static Addr am_dpiz()
    {
        Dword offset = getDword(pc);

        // Without this masking, this addressing mode can be exploited to
        // do segment arithmetic. Haven't decided if this is a feature or
        // a bug yet.
        // TODO: revisit this.
        // offset &= 0xfff; // Mask to 12-bits

        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (getQword((Qword)(dp.q + offset)) + z.q);
    }

    /*
    // Direct Page Indirect Long - [d]
    inline static Addr am_dpil()
    {
        Byte disp = getByte(join_w(pbr, pc));

        BYTES(1);
        cycles += 4; // TODO: fix cycles
        return (getAddr(bank(0) | (Word)(dp.w + disp)));
    }

    // Direct Page Indirect Long Indexed - [d],Y
    inline static Addr am_dily()
    {
        Byte disp = getByte(join_w(pbr, pc));

        BYTES(1);
        cycles += 4; // TODO: fix cycles
        return (getAddr(bank(0) | (Word)(dp.w + disp)) + y.w);
    }
    */

    // Implied/Stack
    inline static Addr am_impl()
    {
        BYTES(0);
        return (0);
    }

    // Accumulator
    inline static Addr am_acc()
    {
        BYTES(0);
        return (0);
    }

    // Immediate Byte
    inline static Addr am_immb()
    {
        Addr ea = pc;
        BYTES(1);
        cycles += 0; // TODO: fix cycles
        return (ea);
    }

    // Immediate Word
    inline static Addr am_immw()
    {
        Addr ea = pc;
        BYTES(2);
        cycles += 1; // TODO: fix cycles
        return (ea);
    }

    // Immediate Dword
    inline static Addr am_immd()
    {
        Addr ea = pc;
        BYTES(4);
        cycles += 3; // TODO: fix cycles
        return (pc);
    }

    // Immediate Qword
    inline static Addr am_immq()
    {
        Addr ea = pc;
        BYTES(8);
        cycles += 7; // TODO: fix cycles
        return (ea);
    }

    /*
    // Immediate based on size of A/M
    inline static Addr am_immm()
    {
        Addr ea = join_w (pbr, pc);
        unsigned int size = (e || p.f_m) ? 1 : 2;

        BYTES(size);
        cycles += size - 1; // TODO: fix cycles
        return (ea);
    }

    // Immediate based on size of X/Y
    inline static Addr am_immx()
    {
        Addr ea = join_w(pbr, pc);
        unsigned int size = (e || p.f_x) ? 1 : 2;

        BYTES(size);
        cycles += size - 1; // TODO: fix cycles
        return (ea);
    }
    */

    // Long Relative - d (signed 32 bit displacement, from -2147483648 to +2147483647)
    inline static Addr am_lrel()
    {
        Dword disp = getDword(pc);

        BYTES(4);
        cycles += 2; // TODO: fix cycles
        return (Qword)(pc + (signed long)disp);
    }

    // Relative - d (signed 16 bit displacement, from -32768 to +32767)
    inline static Addr am_rela()
    {
        Word disp = getWord(pc);

        BYTES(2);
        cycles += 1; // TODO: fix cycles
        return (Qword)(pc + (signed short)disp);
    }

    // Stack Relative - d,S (signed 16 bit displacement, from -32768 to +32767)
    inline static Addr am_srel()
    {
        Word disp = getWord(pc);

        BYTES(2);
        cycles += 1; // TODO: fix cycles

        /*
        if (e)
            return((bank(0) | join_w(sp.b + disp, hi_b(sp.w))));
        else
            return (bank(0) | (Word)(sp.w + disp));
        */

        return (Qword)(sp.q + (signed short)disp);
    }

    // Stack Relative Indirect Indexed X - (d,S),X (signed 16 bit displacement, from -32768 to +32767)
    inline static Addr am_srix()
    {
        Word disp = getWord(pc);
        register Qword ia;

        BYTES(2);
        cycles += 3; // TODO: fix cycles

        ia = getQword((Qword)(sp.q + (signed short)disp));

        return (ia + x.q);
    }

    // Stack Relative Indirect Indexed Y - (d,S),Y (signed 16 bit displacement, from -32768 to +32767)
    inline static Addr am_sriy()
    {
        Word disp = getWord(pc);
        register Qword ia;

        BYTES(2);
        cycles += 3; // TODO: fix cycles

        /*

        if (e)
            ia = getWord(join_w(sp.b + disp, hi_b(sp.w)));
        else
            ia = getWord(bank(0) | (sp.w + disp));

        */

        ia = getQword((Qword)(sp.q + (signed short)disp));

        return (ia + y.q);
    }

    // Stack Relative Indirect Indexed Z - (d,S),Z (signed 16 bit displacement, from -32768 to +32767)
    inline static Addr am_sriz()
    {
        Word disp = getWord(pc);
        register Qword ia;

        BYTES(2);
        cycles += 3; // TODO: fix cycles

        ia = getQword((Qword)(sp.q + (signed short)disp));

        return (ia + z.q);
    }

    // Set the Negative flag
    inline static void setn(unsigned int flag)
    {
        p.f_n = flag ? 1 : 0;
    }

    // Set the Overflow flag
    inline static void setv(unsigned int flag)
    {
        p.f_v = flag ? 1 : 0;
    }

    // Set the decimal flag
    inline static void setd(unsigned int flag)
    {
        p.f_d = flag ? 1 : 0;
    }

    // Set the Interrupt Disable flag
    inline static void seti(unsigned int flag)
    {
        p.f_i = flag ? 1 : 0;
    }

    // Set the Zero flag
    inline static void setz(unsigned int flag)
    {
        p.f_z = flag ? 1 : 0;
    }

    // Set the Carry flag
    inline static void setc(unsigned int flag)
    {
        p.f_c = flag ? 1 : 0;
    }

    // Set the Negative and Zero flags from a byte value
    inline static void setnz_b(Byte value)
    {
        setn(value & 0x80);
        setz(value == 0);
    }

    // Set the Negative and Zero flags from a word value
    inline static void setnz_w(Word value)
    {
        setn(value & 0x8000);
        setz(value == 0);
    }

    // Set the Negative and Zero flags from a dword value
    inline static void setnz_d(Dword value)
    {
        setn(value & 0x80000000);
        setz(value == 0);
    }

    // Set the Negative and Zero flags from a qword value
    inline static void setnz_q(Qword value)
    {
        setn(value & 0x8000000000000000);
        setz(value == 0);
    }

    inline static void op_adc(Addr ea)
    {
        TRACE("ADC");
        /*
        if (e || p.f_m) {
            Byte    data = getByte(ea);
            Word    temp = a.b + data + p.f_c;

            if (p.f_d) {
                if ((temp & 0x0f) > 0x09) temp += 0x06;
                if ((temp & 0xf0) > 0x90) temp += 0x60;
            }

            setc(temp & 0x100);
            setv((~(a.b ^ data)) & (a.b ^ temp) & 0x80);
            setnz_b(a.b = lo_b(temp));
            cycles += 2;
        }
        else {
            Word    data = getWord(ea);
            int     temp = a.w + data + p.f_c;

            if (p.f_d) {
                if ((temp & 0x000f) > 0x0009) temp += 0x0006;
                if ((temp & 0x00f0) > 0x0090) temp += 0x0060;
                if ((temp & 0x0f00) > 0x0900) temp += 0x0600;
                if ((temp & 0xf000) > 0x9000) temp += 0x6000;
            }

            setc(temp & 0x10000);
            setv((~(a.w ^ data)) & (a.w ^ temp) & 0x8000);
            setnz_w(a.w = (Word)temp);
            cycles += 2;
        }
        */

        Qword data = getQword(ea);
        Qword temp = 0, temph = 0, templ = 0;
        temph = hi_d(a.q) + hi_d(data);
        templ = lo_d(a.q) + lo_d(data) + p.f_c;

        if (p.f_d) {
            if ((templ & 0x0000000f) > 0x00000009) templ += 0x00000006;
            if ((templ & 0x000000f0) > 0x00000090) templ += 0x00000060;
            if ((templ & 0x00000f00) > 0x00000900) templ += 0x00000600;
            if ((templ & 0x0000f000) > 0x00009000) templ += 0x00006000;
            if ((templ & 0x000f0000) > 0x00090000) templ += 0x00060000;
            if ((templ & 0x00f00000) > 0x00900000) templ += 0x00600000;
            if ((templ & 0x0f000000) > 0x09000000) templ += 0x06000000;
            if ((templ & 0xf0000000) > 0x90000000) templ += 0x60000000;

            if ((temph & 0x0000000f) > 0x00000009) temph += 0x00000006;
            if ((temph & 0x000000f0) > 0x00000090) temph += 0x00000060;
            if ((temph & 0x00000f00) > 0x00000900) temph += 0x00000600;
            if ((temph & 0x0000f000) > 0x00009000) temph += 0x00006000;
            if ((temph & 0x000f0000) > 0x00090000) temph += 0x00060000;
            if ((temph & 0x00f00000) > 0x00900000) temph += 0x00600000;
            if ((temph & 0x0f000000) > 0x09000000) temph += 0x06000000;
            if ((temph & 0xf0000000) > 0x90000000) temph += 0x60000000;
        }

        setc(temph & 0x100000000);
        temp = join_q((Dword)templ, (Dword)temph);
        setv((~(a.q ^ data)) & (a.q ^ temp) & 0x800000000000000);
        setnz_q(a.q = temp);
        cycles += 2; // TODO: fix cycles
    }

    inline static void op_and(Addr ea)
    {
        TRACE("AND");
        /*
        if (e || p.f_m) {
            setnz_b(a.b &= getByte(ea));
            cycles += 2;
        }
        else {
            setnz_w(a.w &= getWord(ea));
            cycles += 3;
        }
        */

        setnz_q(a.q &= getQword(ea));
        cycles += 3; // TODO: fix cycles
    }

    inline static void op_asl(Addr ea)
    {
        TRACE("ASL");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setc(data & 0x80);
            setnz_b(data <<= 1);
            setByte(ea, data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setc(data & 0x8000);
            setnz_w(data <<= 1);
            setWord(ea, data);
            cycles += 5;
        }
    }

    inline static void op_asla(Addr ea)
    {
        TRACE("ASL");

        if (e || p.f_m) {
            setc(a.b & 0x80);
            setnz_b(a.b <<= 1);
            setByte(ea, a.b);
        }
        else {
            setc(a.w & 0x8000);
            setnz_w(a.w <<= 1);
            setWord(ea, a.w);
        }
        cycles += 2;
    }

    inline static void op_bcc(Addr ea)
    {
        TRACE("BCC");

        if (p.f_c == 0) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bcs(Addr ea)
    {
        TRACE("BCS");

        if (p.f_c == 1) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_beq(Addr ea)
    {
        TRACE("BEQ");

        if (p.f_z == 1) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bit(Addr ea)
    {
        TRACE("BIT");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setz((a.b & data) == 0);
            setn(data & 0x80);
            setv(data & 0x40);
            cycles += 2;
        }
        else {
            register Word data = getWord(ea);

            setz((a.w & data) == 0);
            setn(data & 0x8000);
            setv(data & 0x4000);

            cycles += 3;
        }
    }

    inline static void op_biti(Addr ea)
    {
        TRACE("BIT");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setz((a.b & data) == 0);
        }
        else {
            register Word data = getWord(ea);

            setz((a.w & data) == 0);
        }
        cycles += 2;
    }

    inline static void op_bmi(Addr ea)
    {
        TRACE("BMI");

        if (p.f_n == 1) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bne(Addr ea)
    {
        TRACE("BNE");

        if (p.f_z == 0) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bpl(Addr ea)
    {
        TRACE("BPL");

        if (p.f_n == 0) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bra(Addr ea)
    {
        TRACE("BRA");

        if (e && ((pc ^ ea) & 0xff00)) ++cycles;
        pc = (Word)ea;
        cycles += 3;
    }

    inline static void op_brk(Addr ea)
    {
        TRACE("BRK");

        if (e) {
            pushWord(pc);
            pushByte(p.b | 0x10);

            p.f_i = 1;
            p.f_d = 0;
            pbr = 0;

            pc = getWord(0xfffe);
            cycles += 7;
        }
        else {
            pushByte(pbr);
            pushWord(pc);
            pushByte(p.b);

            p.f_i = 1;
            p.f_d = 0;
            pbr = 0;

            pc = getWord(0xffe6);
            cycles += 8;
        }
    }

    inline static void op_brl(Addr ea)
    {
        TRACE("BRL");

        pc = (Word)ea;
        cycles += 3;
    }

    inline static void op_bvc(Addr ea)
    {
        TRACE("BVC");

        if (p.f_v == 0) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_bvs(Addr ea)
    {
        TRACE("BVS");

        if (p.f_v == 1) {
            if (e && ((pc ^ ea) & 0xff00)) ++cycles;
            pc = (Word)ea;
            cycles += 3;
        }
        else
            cycles += 2;
    }

    inline static void op_clc(Addr ea)
    {
        TRACE("CLC");

        setc(0);
        cycles += 2;
    }

    inline static void op_cld(Addr ea)
    {
        TRACE("CLD")

        setd(0);
        cycles += 2;
    }

    inline static void op_cli(Addr ea)
    {
        TRACE("CLI")

        seti(0);
        cycles += 2;
    }

    inline static void op_clv(Addr ea)
    {
        TRACE("CLD")

        setv(0);
        cycles += 2;
    }

    inline static void op_cmp(Addr ea)
    {
        TRACE("CMP");

        if (e || p.f_m) {
            Byte    data = getByte(ea);
            Word    temp = a.b - data;

            setc(temp & 0x100);
            setnz_b(lo_b(temp));
            cycles += 2;
        }
        else {
            Word    data = getWord(ea);
            Addr    temp = a.w - data;

            setc(temp & 0x10000L);
            setnz_w((Word)temp);
            cycles += 3;
        }
    }

    inline static void op_cop(Addr ea)
    {
        TRACE("COP");

        if (e) {
            pushWord(pc);
            pushByte(p.b);

            p.f_i = 1;
            p.f_d = 0;
            pbr = 0;

            pc = getWord(0xfff4);
            cycles += 7;
        }
        else {
            pushByte(pbr);
            pushWord(pc);
            pushByte(p.b);

            p.f_i = 1;
            p.f_d = 0;
            pbr = 0;

            pc = getWord(0xffe4);
            cycles += 8;
        }
    }

    inline static void op_cpx(Addr ea)
    {
        TRACE("CPX");

        if (e || p.f_x) {
            Byte    data = getByte(ea);
            Word    temp = x.b - data;

            setc(temp & 0x100);
            setnz_b(lo_b(temp));
            cycles += 2;
        }
        else {
            Word    data = getWord(ea);
            Addr    temp = x.w - data;

            setc(temp & 0x10000);
            setnz_w((Word) temp);
            cycles += 3;
        }
    }

    inline static void op_cpy(Addr ea)
    {
        TRACE("CPY");

        if (e || p.f_x) {
            Byte    data = getByte(ea);
            Word    temp = y.b - data;

            setc(temp & 0x100);
            setnz_b(lo_b(temp));
            cycles += 2;
        }
        else {
            Word    data = getWord(ea);
            Addr    temp = y.w - data;

            setc(temp & 0x10000);
            setnz_w((Word) temp);
            cycles += 3;
        }
    }

    inline static void op_dec(Addr ea)
    {
        TRACE("DEC");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setByte(ea, --data);
            setnz_b(data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setWord(ea, --data);
            setnz_w(data);
            cycles += 5;
        }
    }

    inline static void op_deca(Addr ea)
    {
        TRACE("DEC");

        if (e || p.f_m)
            setnz_b(--a.b);
        else
            setnz_w(--a.w);

        cycles += 2;
    }

    inline static void op_dex(Addr ea)
    {
        TRACE("DEX");

        if (e || p.f_x)
            setnz_b(x.b -= 1);
        else
            setnz_w(x.w -= 1);

        cycles += 2;
    }

    inline static void op_dey(Addr ea)
    {
        TRACE("DEY");

        if (e || p.f_x)
            setnz_b(y.b -= 1);
        else
            setnz_w(y.w -= 1);

        cycles += 2;
    }

    inline static void op_eor(Addr ea)
    {
        TRACE("EOR");

        if (e || p.f_m) {
            setnz_b(a.b ^= getByte(ea));
            cycles += 2;
        }
        else {
            setnz_w(a.w ^= getWord(ea));
            cycles += 3;
        }
    }

    inline static void op_inc(Addr ea)
    {
        TRACE("INC");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setByte(ea, ++data);
            setnz_b(data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setWord(ea, ++data);
            setnz_w(data);
            cycles += 5;
        }
    }

    inline static void op_inca(Addr ea)
    {
        TRACE("INC");

        if (e || p.f_m)
            setnz_b(++a.b);
        else
            setnz_w(++a.w);

        cycles += 2;
    }

    inline static void op_inx(Addr ea)
    {
        TRACE("INX");

        if (e || p.f_x)
            setnz_b(++x.b);
        else
            setnz_w(++x.w);

        cycles += 2;
    }

    inline static void op_iny(Addr ea)
    {
        TRACE("INY");

        if (e || p.f_x)
            setnz_b(++y.b);
        else
            setnz_w(++y.w);

        cycles += 2;
    }

    inline static void op_jmp(Addr ea)
    {
        TRACE("JMP");

        pbr = lo_b(ea >> 16);
        pc = (Word)ea;
        cycles += 1;
    }

    inline static void op_jsl(Addr ea)
    {
        TRACE("JSL");

        pushByte(pbr);
        pushWord(pc - 1);

        pbr = lo_b(ea >> 16);
        pc = (Word)ea;
        cycles += 5;
    }

    inline static void op_jsr(Addr ea)
    {
        TRACE("JSR");

        pushWord(pc - 1);

        pc = (Word)ea;
        cycles += 4;
    }

    inline static void op_lda(Addr ea)
    {
        TRACE("LDA");

        if (e || p.f_m) {
            setnz_b(a.b = getByte(ea));
            cycles += 2;
        }
        else {
            setnz_w(a.w = getWord(ea));
            cycles += 3;
        }
    }

    inline static void op_ldx(Addr ea)
    {
        TRACE("LDX");

        if (e || p.f_x) {
            setnz_b(lo_b(x.w = getByte(ea)));
            cycles += 2;
        }
        else {
            setnz_w(x.w = getWord(ea));
            cycles += 3;
        }
    }

    inline static void op_ldy(Addr ea)
    {
        TRACE("LDY");

        if (e || p.f_x) {
            setnz_b(lo_b(y.w = getByte(ea)));
            cycles += 2;
        }
        else {
            setnz_w(y.w = getWord(ea));
            cycles += 3;
        }
    }

    inline static void op_lsr(Addr ea)
    {
        TRACE("LSR");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setc(data & 0x01);
            setnz_b(data >>= 1);
            setByte(ea, data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setc(data & 0x0001);
            setnz_w(data >>= 1);
            setWord(ea, data);
            cycles += 5;
        }
    }

    inline static void op_lsra(Addr ea)
    {
        TRACE("LSR");

        if (e || p.f_m) {
            setc(a.b & 0x01);
            setnz_b(a.b >>= 1);
            setByte(ea, a.b);
        }
        else {
            setc(a.w & 0x0001);
            setnz_w(a.w >>= 1);
            setWord(ea, a.w);
        }
        cycles += 2;
    }

    inline static void op_mvn(Addr ea)
    {
        TRACE("MVN");

        Byte src = getByte(ea + 1);
        Byte dst = getByte(ea + 0);

        setByte(join_w(dbr = dst, y.w++), getByte(join_w(src, x.w++)));
        if (--a.w != 0xffff) pc -= 3;
        cycles += 7;
    }

    inline static void op_mvp(Addr ea)
    {
        TRACE("MVP");

        Byte src = getByte(ea + 1);
        Byte dst = getByte(ea + 0);

        setByte(join_w(dbr = dst, y.w--), getByte(join_w(src, x.w--)));
        if (--a.w != 0xffff) pc -= 3;
        cycles += 7;
    }

    inline static void op_nop(Addr ea)
    {
        TRACE("NOP");

        cycles += 2;
    }

    inline static void op_ora(Addr ea)
    {
        TRACE("ORA");

        if (e || p.f_m) {
            setnz_b(a.b |= getByte(ea));
            cycles += 2;
        }
        else {
            setnz_w(a.w |= getWord(ea));
            cycles += 3;
        }
    }

    inline static void op_pea(Addr ea)
    {
        TRACE("PEA");

        pushWord(getWord(ea));
        cycles += 5;
    }

    inline static void op_pei(Addr ea)
    {
        TRACE("PEI");

        pushWord(getWord(ea));
        cycles += 6;
    }

    inline static void op_per(Addr ea)
    {
        TRACE("PER");

        pushWord((Word) ea);
        cycles += 6;
    }

    inline static void op_pha(Addr ea)
    {
        TRACE("PHA");

        if (e || p.f_m) {
            pushByte(a.b);
            cycles += 3;
        }
        else {
            pushWord(a.w);
            cycles += 4;
        }
    }

    inline static void op_phb(Addr ea)
    {
        TRACE("PHB");

        pushByte(dbr);
        cycles += 3;
    }

    inline static void op_phd(Addr ea)
    {
        TRACE("PHD");

        pushWord(dp.w);
        cycles += 4;
    }

    inline static void op_phk(Addr ea)
    {
        TRACE("PHK");

        pushByte(pbr);
        cycles += 3;
    }

    inline static void op_php(Addr ea)
    {
        TRACE("PHP");

        pushByte(p.b);
        cycles += 3;
    }

    inline static void op_phx(Addr ea)
    {
        TRACE("PHX");

        if (e || p.f_x) {
            pushByte(x.b);
            cycles += 3;
        }
        else {
            pushWord(x.w);
            cycles += 4;
        }
    }

    inline static void op_phy(Addr ea)
    {
        TRACE("PHY");

        if (e || p.f_x) {
            pushByte(y.b);
            cycles += 3;
        }
        else {
            pushWord(y.w);
            cycles += 4;
        }
    }

    inline static void op_pla(Addr ea)
    {
        TRACE("PLA");

        if (e || p.f_m) {
            setnz_b(a.b = pullByte());
            cycles += 4;
        }
        else {
            setnz_w(a.w = pullWord());
            cycles += 5;
        }
    }

    inline static void op_plb(Addr ea)
    {
        TRACE("PLB");

        setnz_b(dbr = pullByte());
        cycles += 4;
    }

    inline static void op_pld(Addr ea)
    {
        TRACE("PLD");

        setnz_w(dp.w = pullWord());
        cycles += 5;
    }

    inline static void op_plk(Addr ea)
    {
        TRACE("PLK");

        setnz_b(dbr = pullByte());
        cycles += 4;
    }

    inline static void op_plp(Addr ea)
    {
        TRACE("PLP");

        if (e)
            p.b = pullByte() | 0x30;
        else {
            p.b = pullByte();

            if (p.f_x) {
                x.w = x.b;
                y.w = y.b;
            }
        }
        cycles += 4;
    }

    inline static void op_plx(Addr ea)
    {
        TRACE("PLX");

        if (e || p.f_x) {
            setnz_b(lo_b(x.w = pullByte()));
            cycles += 4;
        }
        else {
            setnz_w(x.w = pullWord());
            cycles += 5;
        }
    }

    inline static void op_ply(Addr ea)
    {
        TRACE("PLY");

        if (e || p.f_x) {
            setnz_b(lo_b(y.w = pullByte()));
            cycles += 4;
        }
        else {
            setnz_w(y.w = pullWord());
            cycles += 5;
        }
    }

    inline static void op_rep(Addr ea)
    {
        TRACE("REP");

        p.b &= ~getByte(ea);
        if (e) p.f_m = p.f_x = 1;
        cycles += 3;
    }

    inline static void op_rol(Addr ea)
    {
        TRACE("ROL");

        if (e || p.f_m) {
            register Byte data = getByte(ea);
            register Byte carry = p.f_c ? 0x01 : 0x00;

            setc(data & 0x80);
            setnz_b(data = (data << 1) | carry);
            setByte(ea, data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);
            register Word carry = p.f_c ? 0x0001 : 0x0000;

            setc(data & 0x8000);
            setnz_w(data = (data << 1) | carry);
            setWord(ea, data);
            cycles += 5;
        }
    }

    inline static void op_rola(Addr ea)
    {
        TRACE("ROL");

        if (e || p.f_m) {
            register Byte carry = p.f_c ? 0x01 : 0x00;

            setc(a.b & 0x80);
            setnz_b(a.b = (a.b << 1) | carry);
        }
        else {
            register Word carry = p.f_c ? 0x0001 : 0x0000;

            setc(a.w & 0x8000);
            setnz_w(a.w = (a.w << 1) | carry);
        }
        cycles += 2;
    }

    inline static void op_ror(Addr ea)
    {
        TRACE("ROR");

        if (e || p.f_m) {
            register Byte data = getByte(ea);
            register Byte carry = p.f_c ? 0x80 : 0x00;

            setc(data & 0x01);
            setnz_b(data = (data >> 1) | carry);
            setByte(ea, data);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);
            register Word carry = p.f_c ? 0x8000 : 0x0000;

            setc(data & 0x0001);
            setnz_w(data = (data >> 1) | carry);
            setWord(ea, data);
            cycles += 5;
        }
    }

    inline static void op_rora(Addr ea)
    {
        TRACE("ROR");

        if (e || p.f_m) {
            register Byte carry = p.f_c ? 0x80 : 0x00;

            setc(a.b & 0x01);
            setnz_b(a.b = (a.b >> 1) | carry);
        }
        else {
            register Word carry = p.f_c ? 0x8000 : 0x0000;

            setc(a.w & 0x0001);
            setnz_w(a.w = (a.w >> 1) | carry);
        }
        cycles += 2;
    }

    inline static void op_rti(Addr ea)
    {
        TRACE("RTI");

        if (e) {
            p.b = pullByte();
            pc = pullWord();
            cycles += 6;
        }
        else {
            p.b = pullByte();
            pc = pullWord();
            pbr = pullByte();
            cycles += 7;
        }
        p.f_i = 0;
    }

    inline static void op_rtl(Addr ea)
    {
        TRACE("RTL");

        pc = pullWord() + 1;
        pbr = pullByte();
        cycles += 6;
    }

    inline static void op_rts(Addr ea)
    {
        TRACE("RTS");

        pc = pullWord() + 1;
        cycles += 6;
    }

    inline static void op_sbc(Addr ea)
    {
        TRACE("SBC");

        if (e || p.f_m) {
            Byte    data = ~getByte(ea);
            Word    temp = a.b + data + p.f_c;

            if (p.f_d) {
                if ((temp & 0x0f) > 0x09) temp += 0x06;
                if ((temp & 0xf0) > 0x90) temp += 0x60;
            }

            setc(temp & 0x100);
            setv((~(a.b ^ data)) & (a.b ^ temp) & 0x80);
            setnz_b(a.b = lo_b(temp));
            cycles += 2;
        }
        else {
            Word    data = ~getWord(ea);
            int     temp = a.w + data + p.f_c;

            if (p.f_d) {
                if ((temp & 0x000f) > 0x0009) temp += 0x0006;
                if ((temp & 0x00f0) > 0x0090) temp += 0x0060;
                if ((temp & 0x0f00) > 0x0900) temp += 0x0600;
                if ((temp & 0xf000) > 0x9000) temp += 0x6000;
            }

            setc(temp & 0x10000);
            setv((~(a.w ^ data)) & (a.w ^ temp) & 0x8000);
            setnz_w(a.w = (Word)temp);
            cycles += 3;
        }
    }

    inline static void op_sec(Addr ea)
    {
        TRACE("SEC");

        setc(1);
        cycles += 2;
    }

    inline static void op_sed(Addr ea)
    {
        TRACE("SED");

        setd(1);
        cycles += 2;
    }

    inline static void op_sei(Addr ea)
    {
        TRACE("SEI");

        seti(1);
        cycles += 2;
    }

    inline static void op_sep(Addr ea)
    {
        TRACE("SEP");

        p.b |= getByte(ea);
        if (e) p.f_m = p.f_x = 1;

        if (p.f_x) {
            x.w = x.b;
            y.w = y.b;
        }
        cycles += 3;
    }

    inline static void op_sta(Addr ea)
    {
        TRACE("STA");

        if (e || p.f_m) {
            setByte(ea, a.b);
            cycles += 2;
        }
        else {
            setWord(ea, a.w);
            cycles += 3;
        }
    }

    inline static void op_stp(Addr ea)
    {
        TRACE("STP");

        if (!interrupted) {
            pc -= 1;
        }
        else
            interrupted = false;

        cycles += 3;
    }

    inline static void op_stx(Addr ea)
    {
        TRACE("STX");

        if (e || p.f_x) {
            setByte(ea, x.b);
            cycles += 2;
        }
        else {
            setWord(ea, x.w);
            cycles += 3;
        }
    }

    inline static void op_sty(Addr ea)
    {
        TRACE("STY");

        if (e || p.f_x) {
            setByte(ea, y.b);
            cycles += 2;
        }
        else {
            setWord(ea, y.w);
            cycles += 3;
        }
    }

    inline static void op_stz(Addr ea)
    {
        TRACE("STZ");

        if (e || p.f_m) {
            setByte(ea, 0);
            cycles += 2;
        }
        else {
            setWord(ea, 0);
            cycles += 3;
        }
    }

    inline static void op_tax(Addr ea)
    {
        TRACE("TAX");

        if (e || p.f_x)
            setnz_b(lo_b(x.w = a.b));
        else
            setnz_w(x.w = a.w);

        cycles += 2;
    }

    inline static void op_tay(Addr ea)
    {
        TRACE("TAY");

        if (e || p.f_x)
            setnz_b(lo_b(y.w = a.b));
        else
            setnz_w(y.w = a.w);

        cycles += 2;
    }

    inline static void op_tcd(Addr ea)
    {
        TRACE("TCD");

        dp.w = a.w;
        cycles += 2;
    }

    inline static void op_tdc(Addr ea)
    {
        TRACE("TDC");

        if (e || p.f_m)
            setnz_b(lo_b(a.w = dp.w));
        else
            setnz_w(a.w = dp.w);

        cycles += 2;
    }

    inline static void op_tcs(Addr ea)
    {
        TRACE("TCS");

        sp.w = e ? (0x0100 | a.b) : a.w;
        cycles += 2;
    }

    inline static void op_trb(Addr ea)
    {
        TRACE("TRB");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setByte(ea, data & ~a.b);
            setz((a.b & data) == 0);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setWord(ea, data & ~a.w);
            setz((a.w & data) == 0);
            cycles += 5;
        }
    }

    inline static void op_tsb(Addr ea)
    {
        TRACE("TSB");

        if (e || p.f_m) {
            register Byte data = getByte(ea);

            setByte(ea, data | a.b);
            setz((a.b & data) == 0);
            cycles += 4;
        }
        else {
            register Word data = getWord(ea);

            setWord(ea, data | a.w);
            setz((a.w & data) == 0);
            cycles += 5;
        }
    }

    inline static void op_tsc(Addr ea)
    {
        TRACE("TSC");

        if (e || p.f_m)
            setnz_b(lo_b(a.w = sp.w));
        else
            setnz_w(a.w = sp.w);

        cycles += 2;
    }

    inline static void op_tsx(Addr ea)
    {
        TRACE("TSX");

        if (e)
            setnz_b(x.b = sp.b);
        else
            setnz_w(x.w = sp.w);

        cycles += 2;
    }

    inline static void op_txa(Addr ea)
    {
        TRACE("TXA");

        if (e || p.f_m)
            setnz_b(a.b = x.b);
        else
            setnz_w(a.w = x.w);

        cycles += 2;
    }

    inline static void op_txs(Addr ea)
    {
        TRACE("TXS");

        if (e)
            sp.w = 0x0100 | x.b;
        else
            sp.w = x.w;

        cycles += 2;
    }

    inline static void op_txy(Addr ea)
    {
        TRACE("TXY");

        if (e || p.f_x)
            setnz_b(lo_b(y.w = x.w));
        else
            setnz_w(y.w = x.w);

        cycles += 2;
    }

    inline static void op_tya(Addr ea)
    {
        TRACE("TYA");

        if (e || p.f_m)
            setnz_b(a.b = y.b);
        else
            setnz_w(a.w = y.w);

        cycles += 2;
    }

    inline static void op_tyx(Addr ea)
    {
        TRACE("TYX");

        if (e || p.f_x)
            setnz_b(lo_b(x.w = y.w));
        else
            setnz_w(x.w = y.w);

        cycles += 2;
    }

    inline static void op_wai(Addr ea)
    {
        TRACE("WAI");

        if (!interrupted) {
            pc -= 1;
        }
        else
            interrupted = false;

        cycles += 3;
    }

    inline static void op_wdm(Addr ea)
    {
        TRACE("WDM");

        switch (getByte(ea)) {
        case 0x01:  std::cout << (char) a.b; break;
        case 0x02:  std::cin >> a.b; break;
        case 0xff:  stopped = true;  break;
        }
        cycles += 3;
    }

    inline static void op_xba(Addr ea)
    {
        TRACE("XBA");

        a.w = swap_w(a.w);
        setnz_b(a.b);
        cycles += 3;
    }

    inline static void op_xce(Addr ea)
    {
        TRACE("XCE");

        unsigned char   oe = e;

        e = p.f_c;
        p.f_c = oe;

        if (e) {
            p.b |= 0x30;
            sp.w = 0x0100 | sp.b;
        }
        cycles += 2;
    }
};

// Rust ffi wrappers
extern "C" {
    void emu65x64_setMemory(unsigned long long memMask, unsigned long long ramSize, const unsigned char *pROM) {
        emu65x64::setMemory(memMask, ramSize, pROM);
    }

    void emu65x64_setMemoryRam(unsigned long long memMask, unsigned long long ramSize, unsigned char *pRAM, const unsigned char *pROM) {
        emu65x64::setMemory(memMask, ramSize, pRAM, pROM);
    }

    void emu65x64_reset(bool trace) {
        emu65x64::reset(trace);
    }

    void emu65x64_step() {
        emu65x64::step();
    }

    unsigned long emu65x64_getCycles() {
        return emu65x64::getCycles();
    }

    bool emu65x64_isStopped() {
        return emu65x64::isStopped();
    }

    void emu65x64_setPc(unsigned long long pc) {
        emu65x64::pc = (emu65x64::Qword)pc;
    }
}
#endif
