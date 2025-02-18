# Platform-specific compilation options
ifeq ($(PLATFORM),mars)
    CFLAGS = -DCONFIG_MARS -march=rv64gc_zba_zbb -mcpu=sifive-u74 -mtune=sifive-7-series
    LDFLAGS += --defsym=KERNBASE=0x40200000
else ifeq ($(PLATFORM),qemu)
    CFLAGS = -DCONFIG_QEMU -march=rv64gc
    LDFLAGS += --defsym=KERNBASE=0x80200000
else
    $(error "PLATFORM must be 'qemu' or 'mars'")
endif

# Build profile-specific compilation options
ifeq ($(PROFILE),debug)
    CFLAGS += -O -ggdb -fno-omit-frame-pointer
else ifeq ($(PROFILE),release)
    CFLAGS += -O2 -fdata-sections -ffunction-sections -fomit-frame-pointer
    ASFLAGS += -O2
else
    $(error "PROFILE must be 'debug' or 'release'")
endif

export CFLAGS LDFLAGS
