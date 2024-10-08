#[cfg(feature = "bevy" )]
pub mod prelude;

#[link(name = "emu65x64")]
extern "C" {
    fn emu65x64_setMemory(memMask: u64, ramSize: u64, pRom: *const u8);
    fn emu65x64_setMemoryRam(memMask: u64, ramSize: u64, pRam: *mut u8, pRom: *const u8);

    fn emu65x64_reset(trace: bool);
    fn emu65x64_step();
    fn emu65x64_getCycles() -> u32;
    fn emu65x64_isStopped() -> bool;
    fn emu65x64_setPc(value: u64);
    /*
    fn emu65x64_getFlags() -> u8;
    fn emu65x64_getPc() -> u64;
    fn emu65x64_getSp() -> u64;
    fn emu65x64_getTp() -> u64;
    fn emu65x64_get_dp() -> u64;
    fn emu65x64_get_a() -> u64;
    fn emu65x64_get_b() -> u64;
    fn emu65x64_get_c() -> u64;
    fn emu65x64_get_x() -> u64;
    fn emu65x64_get_y() -> u64;
    fn emu65x64_get_z() -> u64;

    fn emu65x64_set_flags(flags: u8);
    fn emu65x64_set_pc(value: u64);
    fn emu65x64_set_sp(value: u64);
    fn emu65x64_set_tp(value: u64);
    fn emu65x64_set_dp(value: u64);
    fn emu65x64_set_a(value: u64);
    fn emu65x64_set_b(value: u64);
    fn emu65x64_set_c(value: u64);
    fn emu65x64_set_x(value: u64);
    fn emu65x64_set_y(value: u64);
    fn emu65x64_set_z(value: u64);
     */

    // Memory access

    fn mem65x64_getByteF(addr: u64) -> u8;
    fn mem65x64_getWordF(addr: u64) -> u16;
    fn mem65x64_getDwordF(addr: u64) -> u32;
    fn mem65x64_getQwordF(addr: u64) -> u64;
    fn mem65x64_setByteF(addr: u64, data: u8);
    fn mem65x64_setWordF(addr: u64, data: u16);
    fn mem65x64_setDwordF(addr: u64, data: u32);
    fn mem65x64_setQwordF(addr: u64, data: u64);
}

pub fn set_memory(mem_mask: u64, ram_size: u64, rom: Option<&[u8]>) {
    unsafe {
        let p_rom = match rom {
            Some(rom) => rom.as_ptr(),
            None => std::ptr::null(),
        };

        emu65x64_setMemory(mem_mask, ram_size, p_rom);
    }
}

pub fn set_memory_ram(mem_mask: u64, ram_size: u64, ram: &mut [u8], rom: Option<&[u8]>) {
    unsafe {
        let p_rom = match rom {
            Some(rom) => rom.as_ptr(),
            None => std::ptr::null(),
        };

        emu65x64_setMemoryRam(mem_mask, ram_size, ram.as_mut_ptr(), p_rom);
    }
}

pub fn reset(trace: bool) {
    unsafe {
        emu65x64_reset(trace);
    }
}

pub fn step() {
    unsafe {
        emu65x64_step();
    }
}
/*
pub fn get_flags() -> u8 {
    unsafe {
        emu65x64_get_flags()
    }
}

pub fn get_pc() -> u64 {
    unsafe {
        emu65x64_get_pc()
    }
}

pub fn get_sp() -> u64 {
    unsafe {
        emu65x64_getSp()
    }
}

pub fn get_tp() -> u64 {
    unsafe {
        emu65x64_getTp()
    }
}

pub fn get_dp() -> u64 {
    unsafe {
        emu65x64_get_dp()
    }
}

pub fn get_a() -> u64 {
    unsafe {
        emu65x64_get_a()
    }
}

pub fn get_b() -> u64 {
    unsafe {
        emu65x64_get_b()
    }
}

pub fn get_c() -> u64 {
    unsafe {
        emu65x64_get_c()
    }
}

pub fn get_x() -> u64 {
    unsafe {
        emu65x64_get_x()
    }
}

pub fn get_y() -> u64 {
    unsafe {
        emu65x64_get_y()
    }
}

pub fn get_z() -> u64 {
    unsafe {
        emu65x64_get_z()
    }
}

pub fn set_flags(flags: u8) {
    unsafe {
        emu65x64_set_flags(flags)
    }
}

pub fn set_pc(value: u64) {
    unsafe {
        emu65x64_set_pc(value)
    }
}

pub fn set_sp(value: u64) {
    unsafe {
        emu65x64_set_sp(value)
    }
}

pub fn set_tp(value: u64) {
    unsafe {
        emu65x64_set_tp(value)
    }
}

pub fn set_dp(value: u64) {
    unsafe {
        emu65x64_set_dp(value)
    }
}

pub fn set_a(value: u64) {
    unsafe {
        emu65x64_set_a(value)
    }
}

pub fn set_b(value: u64) {
    unsafe {
        emu65x64_set_b(value)
    }
}

pub fn set_c(value: u64) {
    unsafe {
        emu65x64_set_c(value)
    }
}

pub fn set_x(value: u64) {
    unsafe {
        emu65x64_set_x(value)
    }
}

pub fn set_y(value: u64) {
    unsafe {
        emu65x64_set_y(value)
    }
}

pub fn set_z(value: u64) {
    unsafe {
        emu65x64_set_z(value)
    }
}
 */

pub fn set_pc(value: u64) {
    unsafe {
        emu65x64_setPc(value)
    }
}

// Memory access
#[no_mangle]
extern "C" fn read_byte(addr: u64) -> u8 {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::read_byte(addr)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_getByteF(addr)
        }
    }
}

#[no_mangle]
extern "C" fn read_word(addr: u64) -> u16 {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::read_word(addr)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_getWordF(addr)
        }
    }
}

#[no_mangle]
extern "C" fn read_dword(addr: u64) -> u32 {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::read_dword(addr)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_getDwordF(addr)
        }
    }
}

#[no_mangle]
extern "C" fn read_qword(addr: u64) -> u64 {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::read_qword(addr)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_getQwordF(addr)
        }
    }
}

#[no_mangle]
extern "C" fn write_byte(addr: u64, data: u8) {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::write_byte(addr, data)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_setByteF(addr, data)
        }
    }
}

#[no_mangle]
extern "C" fn write_word(addr: u64, data: u16) {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::write_word(addr, data)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_setWordF(addr, data)
        }
    }
}

#[no_mangle]
extern "C" fn write_dword(addr: u64, data: u32) {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::write_dword(addr, data)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_setDwordF(addr, data)
        }
    }
}

#[no_mangle]
extern "C" fn write_qword(addr: u64, data: u64) {
    unsafe {
        #[cfg(feature = "bevy")]
        {
            prelude::RwMemory::write_qword(addr, data)
        }
        #[cfg(not(feature = "bevy"))]
        {
            mem65x64_setQwordF(addr, data)
        }
    }
}
