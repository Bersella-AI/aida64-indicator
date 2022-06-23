# Arduino-based AIDA64 Indicator

*代号：Epsilon ε*  
在大抑郁时代过后的一次编程尝试。  
本程序用于将Arduino联通电脑上的AIDA64，并使前者显示电脑各硬件的温度、功率，以及内存/显存占用等。

### 硬件需求
- 一块 LCD 2004 液晶屏，支持I²C
- 一块 Arduino Uno 主控板（或其它兼容型号）

### 软件依赖关系
- AIDA64，灵感源自[cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- 基于 VS Code 的插件：Platform IO
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
| 温度 - GPU1 二极管 | `t` | 2 | -9 \~ 99 | 1,1 |
| 功耗 - CPU Package | `P` | 6 | 0 \~ 999.99 | 12,0 |
| 功耗 - GPU1 | `p` | 6 | 0 \~ 999.99 | 11,1 |
| 系统 - GPU1 核心频率 | `f` | 4 | 0 \~ 9999 | 5,0 |
| 系统 - 已用内存 | `M` | 5 | 0 \~ 99999 | 13,2 |
| 系统 - GPU1 专用内存使用 | `m` | 4 | 0 \~ 99999 | 5,2 |
| 温度 - 主板 **MOS** | `B` | 2 | -9 \~ 99 | 6,1 |
| 温度 - 固态硬盘 | `b` | 2 | -9 \~ 99 | 0,2 |

**注意：**

- **不要勾选“显示单位”（Show unit）！**
- ~~软件所示模拟显示器中，1-3 行可容纳至多 20 个字符（包括空格）；第 4 行可容纳 4 个。~~  
  其实这条是单纯考虑到 Serial 的缓冲区“仅含”64 字节罢了……事实上比特率慢至 9600 的话，缓冲区根本来不及填充的罢——16 MHz 频率下，MCU 在两个字节之间足有超万个脉冲的间隔。~~只不过我日常喜欢开 115200 bps，显得它跟我的机器一样迅捷~~
- 每个项目之间必须**至少留出一个空格**。
   + 如某一行的第 20 个字符被使用，则下一行第 1 个字符必须为空格。下例可见 `t 35` 前面有一个空格（因上一行 `P 123.14` 使用了第 20 个字符空间）。
- 温度分类中的“固态硬盘”名称因其型号而异，图标均为“硬盘”。

可于“LCD项目”上方的模拟显示器中预览：

```
T 39 f 1850 P 123.14
 t 35 B 41 p 100.86
b 37 m 5156  M 12345

```

保存设置，将可在LCD 2004屏幕上见到：
```
G:35℃ 1850√ 100.86W
C:39℃ 123.14W
 Mem:12345   MOS:41℃
VRAM: 5156   SSD:37℃
```

### 似乎有些数据欠缺，例如占用百分比……

事实上，像频率、功耗之类的实时状况，可比百分比实用多了。对于处理器，可以通过功耗判断当下是空转，还是只有一个核心发力，还是许多个核心在发力；对于 GPU，因功耗上限恒定（需事先自行获知或设定），亦可判断其是否满状态运行。况且，这小屏幕容量也有限。  
至于内存和 VRAM，其上限恒定，也没必要展示百分比。看到用了多少便足矣。

当然，可以自行定制要展示的项目，但需要做个取舍——屏幕已经塞得满满当当了。

### 未来的计划

鉴于近期相关硬件价格大涨，可能考虑针对LCD 1602液晶屏进行优化……（摊手）

--------
# English Manual

*Codename: EPSILON ε*  
Just an attempt after the Great Depression.  
The program was designed for **monitoring hardware components on the computer via Arduino**.

### Hardware needed
- An HD44780-powered LCD screen with 4 rows, 20 columns (sold as "LCD 2004" or "LCD 20x4"), with an I²C backpack
- An Arduino Uno (or its equivalents)

### Software dependency
- AIDA64, inspired by [cokesu](http://www.51hei.com/bbs/dpj-188021-1.html)
- Library [LiquidCrystal_I2C](https://github.com/enjoyneering/LiquidCrystal_I2C)
- Platform IO on the Visual Studio Code

### Technical notes
Improvements over *cokesu*'s solution have been made, including:
- Up to 9 items can be transfered. Please see the chart below.
- The ending pattern `E` has been removed, to suffice the 64-byte limit on the Serial's buffer (defined by `Arduino.h`).
- ~~The Serial is configured to run on 115200 bps -- the default 9600 bps is sufficient enough, though.~~ That can be customized if needed :-P

Instructions on AIDA64:

1. Find out the "LCD" section in the "Hardware Monitoring" preferences.
1. Select the tab "Pertelian", then choose "LCD port" to the corresponding COM port for Arduino.  
  For "LCD type", `20x4` must be selected.
1. Turn on **"Enable Pertelian X2040 LCD support"**.
1. Enter the section **"LCD Items"** on the left, then add items that I desire.
   - Changes to their **LABELS** are needed. Please refer to [Selected items](#Selected_items).
   - **Do not SHOW UNITS!**

#### Selected items

| Item | Label | Max. Length | Limit | X,Y |
|-|-|-|-|-|
| Temperatures - CPU Package | `T` | 2 | -9 \~ 99 | 0,0 |
| Temperatures - GPU1 Diode | `t` | 2 | -9 \~ 99 | 1,1 |
| Power Values - CPU | `P` | 5 | 0 \~ 999.99 | 12,0 |
| Power Values - GPU1 | `p` | 6 | 0 \~ 999.99 | 11,1 |
| System - GPU1 Clock | `f` | 4 | 0 \~ 9999 | 5,0 |
| System - Used Memory | `M` | 5 | 0 \~ 99999 | 13,2 |
| GPU1 Used Dedicated Memory | `m` | 5 | 0 \~ 99999 | 5,2 |
| Temp. - Motherboard **MOS** | `B` | 2 | -9 \~ 99 | 6,1 |
| Temperatures - SSD | `b` | 2 | -9 \~ 99 | 0,2 |

The emulated screen on the top of "Preferences" should look like this:
```
B 41 f 1850 P 123.14
 t 35 b 37 p 100.86
T 39 m 5156  M 12345

```
***Note:*** 
- ~~**On the EMULATED "Pertelian X2040" screen,** UP TO 20 patterns can be carried on Line 1-3, and ONLY 4 on Line 4.~~  
  In fact, with the Serial working at 9600 bps (the default baudrate), the MCU would have **plenty of time** (that is, over 10k cycles @ 16 MHz) to process the last transferred byte before another comes. It's worth concerning only when a MUCH higher baudrate is implemented. ~~Yep I prefer 115200 bps as it works as responsive as my gaming rig :-P~~
- At least one space MUST be reserved between each items.
  + In cases when the last charactor on any line has to be utilized, a space MUST be reserved as the first character on the next line. This can be seen ahead of "GPU1 Temp. (`t 35`)" .

Then, on the LCD 2004 screen, they will look like this:
```
G:35℃ 1850√ 100.86W
C:39℃ 123.14W
 Mem:12345   MOS:41℃
VRAM: 5156   SSD:37℃
```

### Why not workload percents?

The REAL statuses on hardware are way more handful & favorable de facto. Through power load in watts, we can figure out whether the CPU/GPU is running hard (they have power limits that never change and can be checked out or manually adjusted), or how many cores in the CPU being utilized. Also, there's limited room for showing info on the LCD screen. As such, I have selected items most handful for daily monitoring.  
For RAM/VRAM, all we need are how many they're BEING USED, isn't it?

Items to be shown can be customized, though. Note that there's little room left on the screen, and thus replacements may be essential.

### Future attempts
- Adoptions for LCD 16x2 screens may be made, due to recent price hikes on all electronic products...