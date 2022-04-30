# make clean    CROSS_COMPILE=arm-linux-gnueabihf-     PLATFORM=vexpress-qemu_virt     TA_DEV_KIT_DIR=~/Documents/hikey/optee_os/compilation_optee_os/optee_os/out/arm/export-ta_arm32
# make    CROSS_COMPILE=arm-linux-gnueabihf-     PLATFORM=vexpress-qemu_virt     TA_DEV_KIT_DIR=~/Documents/hikey/optee_os/compilation_optee_os/optee_os/out/arm/export-ta_arm32

make clean    CROSS_COMPILE=aarch64-linux-gnu-     PLATFORM=vexpress-qemu_virt     TA_DEV_KIT_DIR=~/Documents/hikey/optee_os/compilation_optee_os/optee_os/out/arm/export-ta_arm64
make    CROSS_COMPILE=aarch64-linux-gnu-     PLATFORM=vexpress-qemu_virt     TA_DEV_KIT_DIR=~/Documents/hikey/optee_os/compilation_optee_os/optee_os/out/arm/export-ta_arm64

