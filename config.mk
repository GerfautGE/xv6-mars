# ================================================
# xv6-mars Configuration File
# This file controls the kernel configuration.
# Modify the options below as needed.
# ================================================

# Target platform selection
# Options: qemu, mars
PLATFORM = mars

# Build profile
# Options: debug, release
PROFILE = debug

# Export the configuration variables for the Makefile
export PLATFORM PROFILE
