# Platform-specific compilation options
ifeq ($(PLATFORM),JH7110)
    CFLAGS = -DCONFIG_JH7110 -march=rv64gc_zba_zbb -mcpu=sifive-u74 -mtune=sifive-7-series
    LDFLAGS += --defsym=KERNBASE=0x40200000
else ifeq ($(PLATFORM),QEMU)
    CFLAGS = -DCONFIG_QEMU -march=rv64gc
    LDFLAGS += --defsym=KERNBASE=0x80200000
else
    $(error "PLATFORM must be 'QEMU' or 'JH7110'")
endif

# Build profile-specific compilation options
ifeq ($(PROFILE),DEBUG)
    CFLAGS += -O -ggdb -fno-omit-frame-pointer
else ifeq ($(PROFILE),RELEASE)
    CFLAGS += -O2 -fdata-sections -ffunction-sections -fomit-frame-pointer
    ASFLAGS += -O2
else
    $(error "PROFILE must be 'DEBUG' or 'RELEASE'")
endif

export CFLAGS LDFLAGS
