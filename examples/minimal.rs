fn main() {
    println!("Hello, world!");

    let mut ram: Vec<u8> = vec![0; 0x4000_0000];

    ram[0] = 0x69; // adc q#1
    ram[1] = 1;
    ram[11] = 0xDB; // stp
    ram[12] = 0xDB; // stp
    ram[13] = 0x40; // rti
    ram[14] = 0xDB; // stp
    ram[0x3fffffd8] = 13;

    emu65x64::set_memory_ram(0x3fff_ffff, 0x4000_0000, &mut ram, None);
    emu65x64::reset(true);
    emu65x64::set_pc(0);

    for _ in 0..6 {
        emu65x64::step();
    }
}
