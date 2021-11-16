# Arduino-based AIDA64 Indicator

*代号：Epsilon ε*  
在大抑郁时代过后的一次编程尝试。  
本程序用于将Arduino联通电脑上的AIDA64，并使前者显示电脑各硬件的温度、功率，以及内存/显存占用等。

### 硬件需求
- 一块LCD 2004液晶屏，支持I²C
- 一块Arduino Uno主控板（或其它兼容型号）

### 软件依赖关系
- AIDA64，灵感源自[cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- 基于VS Code的插件：Platform IO
  + 液晶屏驱动库：[LiquidCrystal_I2C](https://github.com/enjoyneering/LiquidCrystal_I2C)

### 使用指南
1. 把Arduino连接到电脑，烧录本程序。
1. 于“设备管理器”的“COM端口”处，找到其串口芯片名称（例如CH340），记住其端口号（如 `COM3`）。
1. 打开AIDA64，进行设置：
   - 在“硬件监视工具”的“LCD”一项，选中 **Pertelian** 选项卡；
   - “LCD端口”选中刚才记住的端口号（如`COM3`）；
   - “LCD类型”必须为 `20x4`；
   - 勾选“启用...支持”。
   - 再在左边进入“LCD项目”，添加以下九种项目：

| 项目 | 标签（Label） | 最大长度 | 量程 | X,Y坐标 |
|-|-|-|-|-|
| 温度 - CPU Package | `T` | 2 | -9 \~ 99 | 0,0 |
| 温度 - GPU1 二极管 | `t` | 2 | -9 \~ 99 | 1,2 |
| 功耗 - CPU Package | `P` | 6 | 0 \~ 999.99 | 12,1 |
| 功耗 - GPU1 | `p` | 6 | 0 \~ 999.99 | 11,2 |
| 系统 - GPU1 核心频率 | `f` | 4 | 0 \~ 9999 | 5,1 |
| 系统 - 已用内存 | `M` | 5 | 0 \~ 99999 | 12,0 |
| 系统 - GPU1 专用内存使用 | `m` | 4 | 0 \~ 9999 | 5,0 |
| 温度 - 主板 **MOS** | `B` | 2 | -9 \~ 99 | 0,1 |
| 温度 - 固态硬盘 | `b` | 2 | -9 \~ 99 | 6,2 |

**注意：**

- **不要勾选“显示单位”（Show unit）！**
- 模拟显示器每行可容纳至多 20 个字符（包括空格）；
- 每个项目之间必须**至少留出一个空格**。
- 温度分类中的“固态硬盘”因其型号而异，图标均为“硬盘”。

可于“LCD项目”上方的模拟显示器中预览：

```
T 39 m 5156 M 12345
B 41 f 1850 P 123.14
 t 35 b 37 p 100.86

```

保存设置，将可在LCD 2004屏幕上见到：
```
G:35℃ 1850√ 100.86W
C:39℃ 123.14W
Mem:12345  VRAM:5156
MOS:41℃    SSD:37℃
```

### 未来的计划

鉴于近期相关硬件价格大涨，可能考虑针对LCD 1602液晶屏进行优化……（摊手）

--------
# English Manual

*Codename: EPSILON ε*  
Just an attempt after the Great Depression.  
The program was designed for **monitoring hardware components on the computer via Arduino**.

### Hardware dependency
- An HD44780-based LCD screen with 4 rows, 20 columns (sold as "LCD 2004" or "LCD 20x4"), with an I²C backpack
- An Arduino Uno (or its equivalents)

### Software dependency
- AIDA64, inspired by [cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- Library [LiquidCrystal_I2C](https://github.com/enjoyneering/LiquidCrystal_I2C)
- Platform IO on the Visual Studio Code

### Technical notes
Improvements over *cokesu*'s solution have been made, including:
- Up to 9 items can be transfered. Please see the chart below.
- The ending pattern `E` has been removed, to suffice the 64-byte limit on the Serial's buffer (defined by `Arduino.h`).
- ~~The Serial is configured to run on 115200 bps -- the default 9600 bps is sufficient enough, though.~~

Instructions on AIDA64:

1. Find out the "LCD" section in the "Hardware Monitoring" preferences.
1. Select the tab "Pertelian", then choose "LCD port" to the corresponding COM port for Arduino.  
  For "LCD type", `20x4` must be selected.
1. Turn on **"Enable Pertelian X2040 LCD support"**.
1. Enter the section **"LCD Items"** on the left, then add items that I desire.
  - For their **labels**, please refer to [Selected items](#Selected_items).
  - **Do not SHOW UNITS!**

#### Selected items

| Item | Label | Max. Length | Limit | X,Y |
|-|-|-|-|-|
| Temperatures - CPU Package | `T` | 2 | -9 \~ 99 | 0,0 |
| Temperatures - GPU1 Diode | `t` | 2 | -9 \~ 99 | 1,2 |
| Power Values - CPU | `P` | 5 | 0 \~ 999.99 | 12,1 |
| Power Values - GPU1 | `p` | 6 | 0 \~ 999.99 | 11,2 |
| System - GPU1 Clock | `f` | 4 | 0 \~ 9999 | 5,1 |
| System - Used Memory | `M` | 5 | 0 \~ 99999 | 12,0 |
| GPU1 Used Dedicated Memory | `m` | 4 | 0 \~ 9999 | 5,0 |
| Temp. - Motherboard **MOS** | `B` | 2 | -9 \~ 99 | 0,1 |
| Temperatures - SSD | `b` | 2 | -9 \~ 99 | 6,2 |

The preview table on the top of "Preferences" should look like this:
```
T 39 m 5156 M 12345
B 41 f 1850 P 123.14
 t 35 b 37 p 100.86

```
***Note:*** 
- All lines (except the last one) can carry UP TO 20 patterns; 
- At least one space MUST be reserved between each items.
  + In cases when the last space on any line has to be utilized, the first space on the next line MUST be reserved. Please refer to the table above for their recommended X-Y locations.

Then, on the LCD 2004 screen, they will look like this:
```
G:35℃ 1850√ 100.86W
C:39℃ 123.14W
Mem:12345  VRAM:5156
MOS:41℃    SSD:37℃
```


### Future attempts
- Adoptions for LCD 16x2 screens may be made, due to recent price hikes on all electronic products...