# ================================================
# xv6-mars Configuration File
# This file controls the kernel configuration.
# Modify the options below as needed.
# ================================================

# Target platform selection
# Options: JH7110, QEMU
PLATFORM = JH7110

# Build profile
# Options: DEBUG, RELEASE
PROFILE = DEBUG

# Export the configuration variables for the Makefile
export PLATFORM PROFILE
