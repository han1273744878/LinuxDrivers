#!/bin/bash
# GPIO 地址映射查询脚本

echo "=== GPIO 地址映射 ==="
echo ""

for i in 0 1 2 3 4; do
    alias_path="/proc/device-tree/aliases/gpio$i"
    if [ -f "$alias_path" ]; then
        dt_path=$(cat $alias_path)
        addr=$(echo $dt_path | grep -oP 'gpio@\K[0-9a-f]+')
        
        echo "gpio$i (别名) → 地址: 0x$addr → GPIO$((i+1)) (硬件名称)"
    fi
done

echo ""
echo "=== 从 /sys/class/gpio 验证 ==="
ls -l /sys/class/gpio/ | grep gpiochip | while read line; do
    chip=$(echo $line | awk '{print $9}')
    target=$(echo $line | awk '{print $11}')
    addr=$(echo $target | grep -oP '[0-9a-f]{7}\.gpio' | cut -d. -f1)
    echo "$chip → 地址: 0x$addr"
done

echo ""
echo "=== GPIO 编号范围 ==="
cat /sys/kernel/debug/gpio | grep "gpiochip" | head -5

