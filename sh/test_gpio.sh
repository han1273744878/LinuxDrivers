#!/bin/bash
# GPIO 测试脚本 - 测试哪些 GPIO 可用

echo "Testing GPIO availability..."

# 测试 GPIO 范围
for gpio in 0 1 2 3 4 6 7 8 9 10 11 12 13 14 15 16 17 18 \
            32 33 34 35 36 37 38 39 40 \
            64 65 66 67 69 70 71 72 73 74 75; do
    
    # 尝试导出 GPIO
    echo $gpio > /sys/class/gpio/export 2>/dev/null
    
    if [ $? -eq 0 ]; then
        echo "✅ GPIO $gpio - Available"
        # 清理
        echo $gpio > /sys/class/gpio/unexport 2>/dev/null
    else
        echo "❌ GPIO $gpio - In use or unavailable"
    fi
done

