
set -e

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

KERNEL_DIR="/home/han/desktop/code/Linux驱动/linux-4.9.88"
DEFCONFIG=vexpress_defconfig

JOBS=$(nproc)

#开始构建
echo "--> 1.进入内核源码目录: $KERNEL_DIR"
cd "$KERNEL_DIR"

echo "--> 2.清理源码树..."
make mrproper

echo "--> 3.使用 'DEFCONFIG' 配置内核..."
make "$DEFCONFIG"

echo "--> 4.开始编译内核镜像(zImage)、模块和设备树..."
make -j"$JOBS" zImage modules dtbs

echo "========================================"
echo "==> 内核编译成功!"
echo "========================================"
echo "==> 输出文件:"
echo "    内核镜像: $KERNEL_DIR/arch/$ARCH/boot/zImage"
echo "    设备树文件: $KERNEL_DIR/arch/$ARCH/boot/dts/"
echo "    内核模块:   需要运行 'make modules_install' 来安装"