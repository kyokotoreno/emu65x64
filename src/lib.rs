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
