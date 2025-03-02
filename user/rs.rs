#![no_std]
#![no_main]

extern "C" {
    fn printf(format: *const u8, ...) -> i32;
    fn exit(status: i32) -> !;
}

#[no_mangle]
pub extern "C" fn main() {
    unsafe {
        printf(b"Hello from Rust!\n\0" as *const u8);
    }
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    unsafe {
        exit(-1);
    }
}
