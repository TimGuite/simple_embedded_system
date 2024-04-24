#![no_std]
#![no_main]

use esp_backtrace as _;
use esp_hal::{clock::ClockControl, delay::Delay, gpio::IO, i2c::I2C, peripherals::Peripherals, prelude::*};

extern crate alloc;
use core::mem::MaybeUninit;

#[global_allocator]
static ALLOCATOR: esp_alloc::EspHeap = esp_alloc::EspHeap::empty();

fn init_heap() {
    const HEAP_SIZE: usize = 32 * 1024;
    static mut HEAP: MaybeUninit<[u8; HEAP_SIZE]> = MaybeUninit::uninit();

    unsafe {
        ALLOCATOR.init(HEAP.as_mut_ptr() as *mut u8, HEAP_SIZE);
    }
}

#[entry]
fn main() -> ! {
    let peripherals = Peripherals::take();
    let system = peripherals.SYSTEM.split();

    // Set up GPIO 19 and 18 for I2C1
    let io = IO::new(peripherals.GPIO, peripherals.IO_MUX);
    let sda = io.pins.gpio19;
    let scl = io.pins.gpio18;

    let clocks = ClockControl::max(system.clock_control).freeze();
    let delay = Delay::new(&clocks);

    // Setup the I2C driver
    let mut i2c = I2C::new(
        peripherals.I2C0,
        sda,
        scl,
        100.kHz(),
        &clocks,
        None
    );

    init_heap();

    esp_println::logger::init_logger_from_env();

    loop {
        i2c.write(0x38, &[0xAC, 0x33, 0]).ok();
        delay.delay_millis(80);

        let mut data = [0; 7];
        i2c.read(0x38, &mut data).ok();
        log::info!("{:?}", data);

        let mut raw = (data[1] as u32) << 8;
        raw += data[2] as u32;
        raw <<= 4;
        raw += (data[3] >> 4) as u32;
        let hum = raw as f32 * 9.5367431640625e-5; // ==> / 1048576.0 * 100%;

        let mut raw = (data[3] & 0x0F) as u32;
        raw <<= 8;
        raw += data[4] as u32;
        raw <<= 8;
        raw += data[5] as u32;
        let temp = raw as f32 * 1.9073486328125e-4 - 50.0; //  ==> / 1048576.0 * 200 - 50;

        log::info!("Temperature: {:?} degC, Humidity: {:?} %", temp, hum);

        delay.delay(1000.millis());
    }
}
