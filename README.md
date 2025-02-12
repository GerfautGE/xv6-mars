# xv6-mars

An attempt to port [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) to the [MilkV mars SBC](https://milkv.io/mars). As MilkV mars is based on JH7110, **xv6-mars** may be runnable on VisionFive2 or Star64 SBCs.

>[!IMPORTANT]
>This project is still under development.

## Features

**xv6-mars** supports:

- SBI timers
- SBI HSM to start
- Ramdisk
- UART
- Buddy allocator

## Run

### Configuration

You need to configure the kernel to run on the Mars board. Edit `config.mk`:

```makefile
PLATFORM = mars
```
### Build
To get xv6 starting on the board, you need to load manually everything.
compile the kernel and ramdisk image
```bash
make
make fs.img
```

>[!TIP]
>Connect to UART to get output:
> | Pin | GPIO |
> |-----|------|
> | GND |     6|
> | Tx  |     8|
> | Rx  |    10|

### Load
#### SD card
Copy `kernel` and `fs.img` to the mars SD card and then boot the mars.

Then once U-Boot prompt appears:
```bash
load mmc 1 ${loadaddr} kernel
load mmc 1 0x49000000 fs.img
bootelf ${loadaddr}
```

#### TFTP server

You can setup a DHCP and TFTP server and connect the Mars in the LAN.
Then setup U-Boot:
``` bash
setenv bootcmd='dhcp; tftpboot ${loadaddr} kernel; tftpboot 0x49000000 fs.img; bootelf ${loadaddr};'
saveenv
```

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
