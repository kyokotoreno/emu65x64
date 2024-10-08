use bevy::prelude::*;

pub struct Emu65x64Plugin;

impl Plugin for Emu65x64Plugin {
    fn build(&self, app: &mut App) {

    }
}

pub trait RwMemory: Resource {
    fn read_byte(addr: u64) -> u8;
    fn read_word(addr: u64) -> u16;
    fn read_dword(addr: u64) -> u32;
    fn read_qword(addr: u64) -> u64;

    fn write_byte(addr: u64, value: u8);
    fn write_word(addr: u64, value: u16);
    fn write_dword(addr: u64, value: u32);
    fn write_qword(addr: u64, value: u64);
}

pub struct Processor65x64<T: RwMemory> {
    memory: T
}

fn processor_step_system() {

}
