# kbd_hc32f460

HC32F460 上的 QMK/Vial 键盘固件，使用 FreeRTOS，当前键盘为 `zero64`。
以下以 HC32F460xE（512KB Flash）和 macOS 终端为例。

## 1. 需要下载和安装什么

| 工具 | 用途 | 下载/安装 |
| --- | --- | --- |
| Arm GNU Toolchain，目标为 `arm-none-eabi` | 编译、链接、生成 HEX/BIN、GDB 调试 | [Arm 官方工具链入口](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)。选择对应电脑系统、CPU 架构的裸机工具链；工程现有路径对应 14.2.Rel1 |
| GNU Make 和命令行 shell | 执行 Makefile | macOS 可运行 `xcode-select --install`；其他系统需提供 GNU Make 及 `mkdir`、`rm`、`date` 等命令 |
| Python 3 | 生成 Vial 定义和 UF2 | [Python 官方下载](https://www.python.org/downloads/)。仅使用标准库，不需要第三方 Python 包，必须支持 `lzma` |
| pyOCD，烧录/调试时需要 | SWD 烧录、GDB、RTT | [pyOCD 官方安装说明](https://pyocd.io/docs/installing)，可通过 pipx 安装 |

HC32 驱动、USB 库、FreeRTOS、QMK/Vial 和 RTT 已包含在仓库中，无需另装 QMK CLI、Keil、IAR 来执行 Make 构建。

先安装 pipx，再安装 pyOCD：

```sh
pipx install pyocd
pipx ensurepath
```

重新打开终端检查：

```sh
make --version
python3 --version
python3 -c "import json, lzma"
pyocd --version
```

pipx 安装的 pyOCD 与 `python3 -m pyocd` 可能属于不同环境；本文使用 `pyocd` 命令。
找不到命令时检查 pipx 的应用目录是否已加入 PATH，例如 `$HOME/.local/bin`。

## 2. 修改编译器路径

检查 app 的 [app.mk](app.mk) 和 boot 的 [makefile](../HC32F460_BOOT/makefile) 中的 `PREFIX`。
目前它们使用本机路径，例如：

```make
PREFIX = /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-
```

改成自己的安装路径。`PREFIX` 包含 `arm-none-eabi-`，不是只填 `bin`。
也可以将工具链的 `bin` 目录加入 PATH，再将两个文件改为：

```make
PREFIX = arm-none-eabi-
```

编译时也能直接覆盖，不必改文件：

```sh
make -j4 PREFIX=/实际安装目录/bin/arm-none-eabi-
```

若已配置 PATH，检查：

```sh
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
arm-none-eabi-size --version
```

## 3. 需要检查哪些配置文件

| 文件 | 配置内容 |
| --- | --- |
| [keyboard/zero64/Makefile](keyboard/zero64/Makefile) | 芯片 `DEVICE = HC32F460xE`、`CONFIG_H = config.h`；目标名来自键盘目录名 |
| [keyboard/zero64/config.h](keyboard/zero64/config.h) | VID/PID、厂商和产品名、矩阵行列数与引脚、Vial UID 等 |
| [keyboard/zero64/rules.mk](keyboard/zero64/rules.mk) | Vial、Tap Dance、Combo、NKRO、鼠标键、`CONSOLE_ENABLE` 等功能开关 |
| [keyboard/zero64/keymap.c](keyboard/zero64/keymap.c) | 默认键位，布局宏位于同目录 `keymap_common.h` |
| [keyboard/zero64/vial.json](keyboard/zero64/vial.json) | Vial 界面布局、行列坐标、VID/PID，须与实际配置一致 |
| [app/config/hc32f4xx_conf.h](app/config/hc32f4xx_conf.h) | `APP_START_ADDR`：带 boot 为 `0x10000`，无 boot 为 `0` |
| [Core/linker/HC32F460xE.ld](Core/linker/HC32F460xE.ld) | Flash 链接起点及长度，见后文 |
| [drivers/usb/usb_dev_desc.c](drivers/usb/usb_dev_desc.c) | 序列号 `DEV_SERIALNUMBER_FS_STRING`；厂商、产品名引用 `config.h` |
| [drivers/wear_leveling/wear_leveling_legacy_config.h](drivers/wear_leveling/wear_leveling_legacy_config.h) | 配置保存起点 `CONFIG_FLASH_BASE_START = 0x78000` |
| [drivers/drivers.mk](drivers/drivers.mk) | EEPROM 物理容量 `WEAR_LEVELING_BACKING_SIZE`、逻辑容量 `WEAR_LEVELING_LOGICAL_SIZE` 等编译宏 |
| [app/config/boot_request.h](app/config/boot_request.h) | Ret-SRAM 模式标记，带 boot 时必须与 boot 工程中的独立副本保持一致 |

矩阵引脚编码为 `端口编号 * 16 + 引脚位号`，端口顺序是 A、B、C、D、E、H。
例如 PA7=7、PB10=26、PC14=46、PH2=82；不是芯片封装脚号。
当前扫描方式为行输出低有效、列输入上拉；应避开板上已连接其他功能的网络。

当前系统时钟使用外部 12MHz 高速晶振，MPLL 得到 200MHz，PLLX 为 USB 提供 48MHz。
改变晶振时，同时检查 BSP 中的 `BSP_CLK_Init()`、`drivers/usb/keyboard_usb_board.c` 的 PLLX 参数和
`drivers/cmsis/Device/HDSC/hc32f4xx/Include/system_hc32f460.h` 中的 `XTAL_VALUE`。
仅修改频率宏不会自动重算 PLL 参数。

## 4. 带 boot 的地址配置

512KB Flash、保留末尾 32KB 配置区时：

| 区域 | 地址 | 大小 |
| --- | --- | --- |
| boot | `0x00000～0x0FFFF` | 64KB |
| app | `0x10000～0x77FFF` | 416KB |
| 配置预留区 | `0x78000～0x7FFFF` | 32KB |

目前 EEPROM 默认物理后端为 8KB、逻辑容量为 4KB，配置预留区并未全部使用。

app 的 `Core/linker/HC32F460xE.ld` 应设置：

```ld
FLASH (rx): ORIGIN = 0x00010000, LENGTH = 416K
```

`app/config/hc32f4xx_conf.h` 应设置：

```c
#define APP_START_ADDR 0x10000
```

**若当前链接脚本仍为 `LENGTH = 448K`，请改为 `416K`，否则 app 链接范围会覆盖末尾配置区。**
boot 自身从地址 0 链接，长度为 64KB；UF2 接收端只允许写入 `[0x10000, 0x78000)`。

## 5. 编译 app

从 `HC32F460_KBD` 根目录执行：

```sh
make -C keyboard/zero64 clean
make -C keyboard/zero64 -j4
```

如果工具链已加入 PATH，可以这样覆盖固定路径：

```sh
make -C keyboard/zero64 -j4 PREFIX=arm-none-eabi-
```

构建自动完成：

1. `python/vial_generate_definition.py` 将 `vial.json` 转成 `vial_generated_keyboard_definition.h`。
2. 编译、链接，生成 ELF、MAP、HEX 和 BIN。
3. `python/uf2conv.py` 从 HEX 生成 UF2，Family ID 为 `0xF4604601`，目标地址取自 HEX。

不需要手动执行 Python 脚本。Vial 生成规则直接调用 `python3`；仅设置 `PYTHON=...` 不会替换这条规则的解释器。

`YYYYMM` 为构建当月，输出为：

```text
build/
├── zero64-YYYYMM.bin
├── zero64-YYYYMM.uf2
└── zero64/
    ├── zero64.elf
    ├── zero64.map
    ├── zero64-YYYYMM.hex
    └── *.o / *.d
```

`clean` 当前只清理 `build/zero64/`，不删除根目录中旧月份的 BIN/UF2。
刷写时核对月份和修改时间。修改启动地址或功能宏后，应先 clean 再编译。

## 6. 编译 boot、烧录与 UF2 升级

从 `HC32F460_KBD` 根目录执行：

```sh
make -C ../HC32F460_BOOT clean
make -C ../HC32F460_BOOT -j4 PREFIX=arm-none-eabi-
```

使用固定路径时可将 `PREFIX` 换成自己的路径，或使用 boot makefile 中配置的路径。
boot 输出位于它自己的工程根目录：`hc32_boot.elf`、`hc32_boot.map`、`hc32_boot.hex`、`hc32_boot.bin`。

连接 SWDIO、SWCLK、GND 和探针所需的目标电压参考；复位调试还需连接 NRST。
烧录示例：

```sh
pyocd list
pyocd load -t hc32f460xe ../HC32F460_BOOT/hc32_boot.hex
pyocd reset -t hc32f460xe
```

[pyOCD 内置目标列表](https://pyocd.io/docs/builtin-targets.html) 包含 `hc32f460xe`。
首次没有有效 app 时进入 U 盘；有 app 时通过双击复位（500ms 窗口）或 `QK_BOOT` 软件请求进入。
PC13 已不再参与进入 U 盘的判断。

把 `build/zero64-YYYYMM.uf2` 拖进升级盘。收到全部 UF2 数据块并校验成功、约 2 秒没有新的有效 UF2 写入后，
boot 清零 Ret-SRAM 模式标记并复位启动 app。普通 BIN/HEX 不按 UF2 处理。
更新 boot 必须通过 SWD；仅拖入 app UF2 不会更新 boot。

boot 与 app 各有一份 `config/boot_request.h`，使用 Ret-SRAM 最后 16 字节。
两边地址、魔数需一致，链接脚本必须预留这段内存，启动代码不能清零它。
当前模式选择不使用旧的 `0xFFFC` Flash 标记。完全断电后 RAM 数据不保证保留，该方案不能保证中断电升级的固件完整性。

## 7. 不需要 boot，需要修改哪里

无 boot 时 app 从 Flash 地址 0 启动。**必须同时修改链接地址和中断向量表偏移。**

### 7.1 链接脚本

把 `Core/linker/HC32F460xE.ld` 的 Flash 区域改为：

```ld
FLASH (rx): ORIGIN = 0x00000000, LENGTH = 480K
```

`480K = 0x78000`，仍给配置区保留最后 32KB，不再为 boot 预留前 64KB。
这套分区仅适用于 512KB 的 xE 型号；xC 容量不同，不能直接套用。

### 7.2 中断向量表偏移

把 `app/config/hc32f4xx_conf.h` 改为：

```c
#define APP_START_ADDR 0x00000000
```

`system_hc32f460.c` 的 `SystemInit()` 用 `EFM_BASE + APP_START_ADDR` 设置 `SCB->VTOR`。
只改链接脚本可能使程序从 0 启动，但中断仍跳到旧地址。
无需修改启动汇编中的 `Reset_Handler` 地址，链接器会生成实际入口地址。

### 7.3 软件进入 boot 的行为

`drivers/platform/keyboard_boot.c` 当前写 Ret-SRAM 后复位，需要独立 boot 读取请求。
无 boot 时没有 U 盘升级入口。可以移除键位中的 `QK_BOOT`，或者将函数改成普通复位：

```c
#include "bootloader.h"
#include "hc32_ll.h"

void bootloader_jump(void)
{
    NVIC_SystemReset();
}
```

前两项是从地址 0 启动的必要修改，第三项用于处理不再有效的 boot 入口。
Ret-SRAM 的 16 字节预留可以保留，无需修改启动代码或 SRAM 布局。
配置区起点 `0x78000` 也无需改变。

### 7.4 重新编译和烧录

```sh
make -C keyboard/zero64 clean
make -C keyboard/zero64 -j4 PREFIX=arm-none-eabi-
```

将文件名中的 `YYYYMM` 替换为实际构建月份：

```sh
pyocd load -t hc32f460xe build/zero64/zero64-YYYYMM.hex
pyocd reset -t hc32f460xe
```

HEX 包含目标地址；烧录 BIN 必须指定基址 0：

```sh
pyocd load -t hc32f460xe --base-address 0x0 build/zero64-YYYYMM.bin
```

无 boot 构建仍生成 UF2，但当前配套 boot 禁止写入低于 `0x10000` 的地址，不能通过它安装这种 UF2。
请通过 SWD 烧录。完成后可读取地址 `0x0` 的初始 SP 和复位入口，确认新镜像确实在地址 0。
本文只记录切换方法，不会自动把当前工程切换为无 boot。

## 8. 常见问题

- 编译器不存在：检查 `PREFIX`、PATH，工具链应为 `arm-none-eabi`，而不是 `arm-linux-*`。
- 缺少 `lzma`：安装包含此标准库模块的 Python。
- Vial 布局没更新：核对 `vial.json`、生成头文件和实际刷写固件的修改时间。
- Tap Dance 索引 4 读取失败：`drivers/eeprom.h` 中必须先定义 `TOTAL_EEPROM_BYTE_COUNT`，再包含 `vial.h`，避免模块之间的条目数量不一致；工程已修复该顺序。
- `_write/_read/_close`、RWX 段或 Python 转义提示：构建可能有这些警告，须以 Make 退出状态和实际生成文件判断是否成功。
- 无 boot 仍显示旧 U 盘：确认两处启动地址都已改动、已 clean 重编，并实际烧录了新的 HEX，而非只生成文件。

## 9. 开源许可证

本项目基于 [Vial/QMK](https://github.com/vial-kb/vial-qmk)。与本工程 Vial 核心的声明一致，
基于它们的固件和项目修改按 **GPL-2.0-or-later**（GNU GPL 第 2 版或任何后续版本）提供。
GPL 第 2 版完整文本见 [LICENSE](LICENSE)。各文件的具体版权和许可声明应予保留。

允许使用、修改和商业销售；分发本项目的固件或基于它的产品时，应遵守 GPL 的要求，
包括向接收者提供相应的完整源码和构建所需文件，并保留版权与许可证声明。

第三方组件继续适用其原有许可证，例如 FreeRTOS 的 MIT 和 HC32 驱动的 BSD-3-Clause。
本项目声明不替换或重新授权这些第三方代码。原根目录的 LGPL-2.1 文本已保留在
[LICENSES/LGPL-2.1.txt](LICENSES/LGPL-2.1.txt)，供适用该许可的第三方代码使用。
独立的 `HC32F460_BOOT` 工程不因本说明自动变更许可证。
