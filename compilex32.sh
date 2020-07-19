export CROSS_COMPILE=~/android/toolchain/arm-eabi-4.8/bin/arm-eabi-
	
export ARCH=arm

export SUBARCH=arm

make lineageos_LS5502_defconfig

#make menuconfig

make -j4
