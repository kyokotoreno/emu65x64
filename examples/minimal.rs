fn main() {
    println!("Hello, world!");

    let mut ram: Vec<u8> = vec![0; 0x4000_0000];

    ram[0] = 0x69;
    ram[1] = 1;

    emu65x64::set_memory_ram(0x3fff_ffff, 0x4000_0000, &mut ram, None);
    emu65x64::reset(true);
    emu65x64::set_pc(0);
    emu65x64::step();
    emu65x64::step();
    emu65x64::step();
}
