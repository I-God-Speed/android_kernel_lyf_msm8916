export CROSS_COMPILE=~/android/toolchain/aarch64-linux-android-4.9-uber-master/bin/aarch64-linux-android-
	
export ARCH=arm64

export SUBARCH=arm64

make lineageos_LS5502_defconfig

#make menuconfig

make -j4
