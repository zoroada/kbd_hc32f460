# UF2 boot 升级说明

工具下载、编译、烧录和无 boot 配置见 [工程 README](../../README.md)。
配套工程为同级目录 `HC32F460_BOOT`。

## 使用流程

1. 通过 SWD 烧录 `HC32F460_BOOT/hc32_boot.hex`。
2. 没有有效 app 时直接进入 U 盘；有 app 时双击复位（500ms 窗口）或通过 `QK_BOOT` 进入。
3. 拖入 app 构建生成的 `build/zero64-YYYYMM.uf2`。
4. 全部数据块写入并校验成功、约 2 秒没有新的有效 UF2 写入后，清零 Ret-SRAM 标记并复位启动 app。

PC13 不再判断是否进入 U 盘。当前不使用旧 BIN 内存盘方案，不检查 `0xFFFC` Flash 启动标记。

## 格式和地址

- boot：`0x00000～0x0FFFF`；app：`0x10000～0x77FFF`；配置预留区：`0x78000～0x7FFFF`。
- UF2 Family ID 为 `0xF4604601`，每块 512 字节，Flash 载荷 256 字节。
- 4MiB 虚拟 FAT16 卷，提供 `INFO_UF2.TXT`、`INDEX.HTM`、`CURRENT.UF2`。
- 数据直接写入 app Flash，不使用 RAM 缓存整个固件；8KB 扇区擦除、连续编程、回读校验。
- 升级器禁止写入 boot 和配置区。普通 BIN/HEX 不是此入口的输入格式。
- Ret-SRAM 完全断电后不保证保留，不能靠它保证中断电升级的固件完整性。

## 调试状态

连接 boot 的 ELF 后读取 `boot_update_status`、`update_num_blocks`、`update_num_written`。

| 状态 | 含义 |
| --- | --- |
| `0` | 接收有效数据块 |
| `1` | 检查完成状态及 app 向量 |
| `3` | 成功，准备复位 |
| `-1` | 数据不完整或向量无效 |
| `-3` | UF2 块处理或 Flash 写入失败 |

修改 boot 后必须通过 SWD 重新烧录 boot，拖入 app UF2 不会更新 boot。
