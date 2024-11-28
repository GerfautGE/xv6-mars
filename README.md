# xv6-mars

An attempt to port [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) to the [MilkV mars SBC](https://milkv.io/mars). As MilkV mars is based on JH7110, **xv6-mars** may be runnable on VisionFive2 or Star64 SBCs.

>[!IMPORTANT]
>This project is still under development.

## Features

**xv6-mars** supports:

- SBI timers
- SBI HSM to start 
- a ramdisk


>[!WARNING]
>UART is still not supported in the kernel. SBI console will print the output but **no input is supported for now**.

## Run
To get xv6 starting on the board, you need to load manually everything.
compile the kernel and ramdisk image
```bash
make PLATFORM=MARS
make fs.img
```
copy them to the mars SD card and then boot the mars.
>[!TIP]
>Connect to UART to get output:
> | Pin | GPIO |
> |-----|------|
> | GND |     6|
> | Tx  |     8|
> | Rx  |    10|

Then once U-Boot prompt appears:
```bash
load mmc 1 0xa0000000 kernel
load mmc 1 0x49000000 fs.img
bootelf 0xa0000000
```
This will start the kernel.
## Debug

You can debug the kernel with [GDB](https://sourceware.org/gdb/) via [OpenOCD](https://openocd.org) on a JTAG port I found on the board.

| JTAG Port | Mars GPIO Pin |
|-----------|---------------|
|TMS        |35             |
|TRSTN      |36             |
|TCK        |37             |
|TDI        |38             |
|TDO        |40             |


>[!TIP]
>Load `kernel.sym` in GDB to get sources
