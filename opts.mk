# Platform-specific compilation options
ifeq ($(PLATFORM),mars)
    CFLAGS = -DCONFIG_MARS
    LDFLAGS += --defsym=KERNBASE=0x40200000
else ifeq ($(PLATFORM),qemu)
    CFLAGS = -DCONFIG_QEMU
    LDFLAGS += --defsym=KERNBASE=0x80200000
else
    $(error "PLATFORM must be 'qemu' or 'mars'")
endif

# Build profile-specific compilation options
ifeq ($(PROFILE),debug)
    CFLAGS += -O -ggdb
else ifeq ($(PROFILE),release)
    CFLAGS += -O2
else
    $(error "PROFILE must be 'debug' or 'release'")
endif

export CFLAGS LDFLAGS
