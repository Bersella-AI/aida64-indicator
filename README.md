# Arduino-based AIDA64 Indicator

*代号：Epsilon ε*  
在大抑郁时代过后的一次编程尝试。  
本程序用于将 Arduino 与电脑上的 AIDA64 进行互动，监测电脑各硬件的温度、功率，以及内存/显存占用等。

### 硬件需求
- 一块 LCD 2004 液晶屏，有 I²C 背板
- 一块 Arduino Uno 主控板（或其它兼容型号），带 USB 通讯口

### 软件依赖关系
- AIDA64，灵感源自[cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- [Visual Studio Code](https://code.visualstudio.com)，以及插件 [Platform IO](https://platformio.org)
  + 液晶屏驱动库：[LiquidCrystal_I2C](https://github.com/enjoyneering/LiquidCrystal_I2C)

### 使用指南
1. 把 Arduino 连接到电脑，烧录本程序。
1. 于“设备管理器”的“COM 端口”处，找到其串口芯片名称（例如 CH340），记住其端口号（如 `COM3`）。
1. 打开 AIDA64，进行设置：
   - 在“硬件监视工具”的“LCD”一项，选中 **Pertelian** 选项卡；
   - “LCD 端口”选中刚才记住的端口号（如`COM3`）；
   - “LCD 类型”必须为 `20x4`；
   - 勾选“**启用 Pertelian X2040 LCD 支持**”。
   - 再在左边进入“LCD 项目”，添加以下项目：

| 项目 | 标签（Label） | 最大长度 | 量程 | X,Y坐标 |
|-|-|-|-|-|
| 温度 - CPU Package | `T` | 2 | -9 \~ 99 | 0, 0 |
| 温度 - GPU1 二极管 | `t` | 2 | -9 \~ 99 | 1, 1 |
| 系统 - CPU 使用率 | `L` | 3 | 0 \~ 100 | 6, 0 |
| 功耗 - CPU Package | `P` | 6 | 0 \~ 999.99 | 12, 0 |
| 功耗 - GPU1 | `p` | 6 | 0 \~ 999.99 | 11, 1 |
| 系统 - GPU1 核心频率 | `f` | 4 | 0 \~ 9999 | 5, 2 |
| 系统 - 已用内存 | `M` | 6 | 0 \~ 999999 | 12, 2 |
| 系统 - GPU1 专用内存使用 | `m` | 5 | 0 \~ 99999 | 1, 3 |
| 温度 - 主板 **MOS** | `B` | 2 | -9 \~ 99 | 6, 1 |
| 温度 - 固态硬盘 | `b` | 2 | -9 \~ 99 | 0, 2 |

**注意：**

- **不要勾选“显示单位”（Show unit）！**
- 串口默认波特率为 9600 bps，确保开箱即用；**推荐以 115200 bps 工作，以获得最佳体验**。可于源代码和“设备管理器”自行调整波特率。  
   在 115200 bps 的波特率下，长度 66 字节（594 位）的数据帧虽略超过 Arduino 串口缓存空间——即 64 字节，但可正常读取并显示（唯一例外是初次接收时有 1~2 个项目空缺）。
- 每个项目之间应当**至少留出一个空格**。
   + 如某一行的第 20 个字符被使用，则下一行第 1 个字符必须为空格。下例可见 `t 56` 前面有一个空格（因上一行 `P 123.14` 使用了第 20 个字符空间）。
- 温度分类中的“固态硬盘”名称因其型号而异，图标均为“硬盘”。

可于“LCD 项目”上方的模拟显示器中预览：

```
T 85 f 1920 P 123.14
 t 56 b 54 p 100.86
B 69 L 100 M 113939
m 11785
```

保存设置，将可在 LCD 2004 屏幕上见到：
```
G:56℃ 100.86W 1920√
C:85℃ 123.14W 100%
Mem:113939   MOS:69℃
VRAM:11785   SSD:54℃
```

### 使用限制

- 温度值范围有限，如高于 99 度或低于零下 9 度，显示将出现异常（如 100 度显示为“10 ℃”）。
   + 但是，由于固件会提前介入控温，诸组件温度原则上不会超过 100 度。唯一例外是手动超频时将处理器结温（“温度墙”）设置于 100 度以上。
   + 将来可能考虑加入过热、低温检测和指示。
- 类似地，本程序不适用于 RAM 容量 ≥ 1000 GB，或安装两颗或以上处理器或 GPU 的平台。如有相关需求，请自行修改源代码以适配。

### 许可协议

[MIT](LICENSE)

--------
# English Manual

*Codename: EPSILON ε*  
Just an attempt after my Great Depression.  
The program was designed for **hardware monitoring by setting up an Arduino to interact with AIDA64**.

### Hardware needed
- An HD44780-powered LCD screen with 4 rows, 20 columns (marketed as "LCD 2004" or "LCD 20x4"), with an I²C backpack
- An Arduino Uno (or its equivalents), with a USB port

### Software dependency
- AIDA64, inspired by [cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- [Visual Studio Code](https://code.visualstudio.com) with the addon [Platform IO](https://platformio.org) installed
   + Library [LiquidCrystal_I2C](https://github.com/enjoyneering/LiquidCrystal_I2C)

### Instructions

1. Connect an Arduino to PC with a USB cable.
2. Check out the Serial port used by Arduino in, for example, Device Manager on Windows. It should look like `COM3` or `/dev/ttyUSB0`.
3. Check out whether the baudrate is set to 9600 bps, or reconfigure it if needed.
4. Burn this program.
5. Run AIDA64, and reach out to the "LCD" section in the "Hardware Monitoring" preferences.
6. Open the tab `Pertelian`, then select "LCD port" to the serial port found above.
   For "LCD type", `20x4` should be chosen.
7. Turn on **"Enable Pertelian X2040 LCD support"**.
8. Enter the section **"LCD Items"** on the left, then append the following items.
   - Changes to their **LABELS** are required. Please refer to [Selected items](#Selected_items).
   - **Do not SHOW UNITS!**

#### Selected items

| Item | Label | Max. Length | Limit | X,Y |
|-|-|-|-|-|
| Temperatures - CPU Package | `T` | 2 | -9 \~ 99 | 0,0 |
| Temperatures - GPU1 Diode | `t` | 2 | -9 \~ 99 | 1,1 |
| System - CPU Usage | `L` | 3 | 0 \~ 100 | 6,0 |
| Power Values - CPU | `P` | 5 | 0 \~ 999.99 | 12,0 |
| Power Values - GPU1 | `p` | 6 | 0 \~ 999.99 | 11,1 |
| System - GPU1 Clock | `f` | 4 | 0 \~ 9999 | 5,2 |
| System - Used Memory | `M` | 6 | 0 \~ 999999 | 12,2 |
| GPU1 Used Dedicated Memory | `m` | 5 | 0 \~ 99999 | 1,3 |
| Temp. - Motherboard **MOS** | `B` | 2 | -9 \~ 99 | 6,1 |
| Temperatures - SSD | `b` | 2 | -9 \~ 99 | 0,2 |


The emulated screen on the top of "Preferences" should look like this:
```
T 85 f 1920 P 123.14
 t 56 b 54 p 100.86
B 69 L 100 M 113939
m 11785
```

***Notes:*** 
- **The recommended baudrate for best experience is 115200 bps.** However, the default baudrate has been set to 9600 bps in favor of plug-and-play. You may reconfigure the baudrate both in source code and in Device Manager.  
  At a baudrate of 115200 bps, a packet of 66 bytes (594 bits), despite exceeding the buffer size of serial (64 bytes) on Arduino, was read and displayed with negligible issues (1~2 items were missing on the first time of reading, and were not thereafter).
- At least one space SHOULD be reserved between each items.
  + In cases when the last charactor on any line has to be utilized, a space MUST be reserved as the first character on the next line. This can be seen ahead of "GPU1 Temp. (`t 56`)" .

Then, on the LCD 2004 screen, they will look like this:
```
G:56℃ 100.86W 1920√
C:85℃ 123.14W 100%
Mem:113939   MOS:69℃
VRAM:11785   SSD:54℃
```

### Limitations of Use

- Showing of temperatures ≥ 100° or ≤ -10° Celsius will misbehave, for example, "10 ℃" will be displayed with actual temperature of 100°.
   + However, due to regulation conducted by firmwares, temperatures should be expected to stay within the limits. The only scenario in which temperatures would exceed 100° Celsius, is manual overclocking with Junction Temp. (T<sub>j</sub>) set beyond 100°.
   + Detection for overheating or subcooling may be planned in future.
- The program is not applicable on systems with RAM ≥ 1000 GB, or with 2 or more processors/GPUs. You may customize the displays to monitor them.

### License

[MIT](LICENSE)