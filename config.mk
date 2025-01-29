# ================================================
# xv6-mars Configuration File
# This file controls the kernel configuration.
# Modify the options below as needed.
# ================================================

# Target platform selection
# Options: qemu, mars
PLATFORM = qemu

# Platform-specific compilation options
ifeq ($(PLATFORM),mars)
    CFLAGS += -DCONFIG_MARS
    LDFLAGS += --defsym=KERNBASE=0x40200000
else ifeq ($(PLATFORM),qemu)
    CFLAGS += -DCONFIG_QEMU
    LDFLAGS += --defsym=KERNBASE=0x80200000
else
    $(error "PLATFORM must be 'qemu' or 'mars'")
endif


# Export the configuration variables for the Makefile
export PLATFORM CFLAGS LDFLAGS
