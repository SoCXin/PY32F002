# py32f0-template

* Puya PY32F0 template project for GNU Arm Embedded Toolchain
* Supported programmers: J-Link, DAPLink/PyOCD
* Supported IDE: VSCode

# Puya PY32F0 Family

* PY32F002
  * PY32F002x5(20KB Flash/3KB RAM)
* PY32F002A
  * PY32F002Ax5(20KB Flash/3KB RAM)
* PY32F003
  * PY32F003x4(16KB Flash/2KB RAM), PY32F003x6(32KB Flash/4KB RAM), PY32F003x8(64KB Flash/8KB RAM)
* PY32F030
  * PY32F030x4(16KB Flash/2KB RAM), PY32F030x6(32KB Flash/4KB RAM), PY32F030x7(48KB Flash/6KB RAM), PY32F030x8(64KB Flash/8KB RAM)
* PY32F072
  * PY32F072xB(128KB Flash/16KB RAM)

**Note**

There is high probability that PY32F002A, PY32F003 and PY32F030 share the same core, despite all the parts listed, you can simply treat them all as PY32F030 in coding and programming.

# File Structure

```
├── Build                       # Build results
├── Docs                        # Datesheets and User Manuals
├── Examples
│   ├── FreeRTOS                # FreeRTOS examples
│   ├── HAL                     # HAL library examples
│   └── LL                      # LL(Low Layer) library examples
├── Libraries
│   ├── BSP                     # SysTick delay and printf for debug
│   ├── BSP_LL                  # SysTick delay and printf for debug
│   ├── CMSIS
│   ├── FreeRTOS                # FreeRTOS library
│   ├── LDScripts               # LD files
│   ├── PY32F0xx_HAL_Driver     # MCU peripheral driver
│   └── PY32F0xx_LL_Driver      # MCU low layer peripheral driver
├── Makefile                    # Make config
├── Misc
│   ├── Flash
│   │   ├── Devices             # FLM files
│   │   └── Sources             # Flash algorithm source code
│   └── SVD                     # SVD files
├── README.md
├── rules.mk                    # Pre-defined rules include in Makefile
└── User                        # User application code
```

# Requirements

* PY32F0 EVB or boards of PY32F002/003/030 series
* Programmer
  * J-Link: J-Link OB programmer
  * PyOCD: DAPLink or J-Link
* SEGGER J-Link Software and Documentation pack [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/)
* PyOCD [https://pyocd.io/](https://pyocd.io/)
* GNU Arm Embedded Toolchain

# Building

## 1. Install GNU Arm Embedded Toolchain

Download the toolchain from [Arm GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) according to your pc architecture, extract the files

```bash
sudo mkdir -p /opt/gcc-arm/
sudo tar xvf arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz -C /opt/gcc-arm/
cd /opt/gcc-arm/
sudo chown -R root:root arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/
```



## 2. Option #1: Install SEGGER J-Link

Download and install JLink from [J-Link / J-Trace Downloads](https://www.segger.com/downloads/jlink/).

```bash
# installation command for .deb
sudo dpkg -i JLink_Linux_V770a_x86_64.deb
```
The default installation directory is */opt/SEGGER*

Copy all .FLM files from [Project directory]/Misc/Flash/Devices/Puya to [JLink directory]/Devices/Puya

```bash
cd py32f0-template
sudo cp -r Misc/Flash/Devices/* /opt/SEGGER/JLink/Devices/
```

Edit JLinkDevices.xml

```bash
sudo vi /opt/SEGGER/JLink/JLinkDevices.xml
```
Add the following lines in `<DataBase>` section

```xml
  <!--                 -->
  <!-- Puya            -->
  <!--                 -->
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F002AX5"  WorkRAMAddr="0x20000000" WorkRAMSize="0xC00" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_20K" BaseAddr="0x08000000" MaxSize="0x5000" Loader="Devices/Puya/PY32F0xx_20.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F002X5"  WorkRAMAddr="0x20000000" WorkRAMSize="0xC00" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_20K" BaseAddr="0x08000000" MaxSize="0x5000" Loader="Devices/Puya/PY32F0xx_20.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F003X4"  WorkRAMAddr="0x20000000" WorkRAMSize="0x800" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_16K" BaseAddr="0x08000000" MaxSize="0x4000" Loader="Devices/Puya/PY32F003xx_16.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F003X6"  WorkRAMAddr="0x20000000" WorkRAMSize="0x1000" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_32K" BaseAddr="0x08000000" MaxSize="0x8000" Loader="Devices/Puya/PY32F003xx_32.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F003X8"  WorkRAMAddr="0x20000000" WorkRAMSize="0x2000" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_64K" BaseAddr="0x08000000" MaxSize="0x10000" Loader="Devices/Puya/PY32F003xx_64.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
    <Device>
    <ChipInfo Vendor="Puya" Name="PY32F030X4"  WorkRAMAddr="0x20000000" WorkRAMSize="0x800" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_16K" BaseAddr="0x08000000" MaxSize="0x4000" Loader="Devices/Puya/PY32F030xx_16.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F030X6"  WorkRAMAddr="0x20000000" WorkRAMSize="0x1000" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_32K" BaseAddr="0x08000000" MaxSize="0x8000" Loader="Devices/Puya/PY32F030xx_32.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F030X7"  WorkRAMAddr="0x20000000" WorkRAMSize="0x1800" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_48K" BaseAddr="0x08000000" MaxSize="0xC000" Loader="Devices/Puya/PY32F030xx_48.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
  <Device>
    <ChipInfo Vendor="Puya" Name="PY32F030X8"  WorkRAMAddr="0x20000000" WorkRAMSize="0x2000" Core="JLINK_CORE_CORTEX_M0"/>
    <FlashBankInfo Name="Flash_64K" BaseAddr="0x08000000" MaxSize="0x10000" Loader="Devices/Puya/PY32F030xx_64.FLM" LoaderType="FLASH_ALGO_TYPE_OPEN" AlwaysPresent="1"/>
  </Device>
```


## 2. Option #2: Install PyOCD

Don't install from apt repository, because the version 0.13.1+dfsg-1 is too low for J-Link probe.

Install PyOCD from pip

```bash
pip uninstall pyocd
```
This will install PyOCD into:
```
/home/[user]/.local/bin/pyocd
/home/[user]/.local/bin/pyocd-gdbserver
/home/[user]/.local/lib/python3.10/site-packages/pyocd-0.34.2.dist-info/*
/home/[user]/.local/lib/python3.10/site-packages/pyocd/*
```
In Ubuntu, .profile will take care of the PATH, run `source ~/.profile` to make pyocd command available

## 3. Clone This Repository

Clone this repository to local workspace
```bash
git clone https://github.com/IOsetting/py32f0-template.git
```

## 4. Edit Makefile

Change the settings in Makefile
* make sure **ARM_TOOCHAIN** points to the correct path of arm-none-eabi-gcc
* If you use J-Link, **FLASH_PROGRM** can be jlink or pyocd
* If you use DAPLink, set **FLASH_PROGRM** to pyocd
* ST-LINK is not supported yet. ST-LINK works in Windows Keil5, but I failed to make it work in Ubuntu
* Puya provides two sets of library, HAL lib and LL lib, switch with **USE_LL_LIB** option
* **ENABLE_PRINTF_FLOAT** will add `-u _printf_float` to link options, which will significantly increase the binary size.

```makefile
##### Project #####

PROJECT			?= app
# The path for generated files
BUILD_DIR		= Build


##### Options #####

# Use LL library instead of HAL
USE_LL_LIB ?= n
# Enable printf float %f support, y:yes, n:no
ENABLE_PRINTF_FLOAT	?= n
# Build with CMSIS DSP functions, y:yes, n:no
USE_DSP			?= n
# Programmer, jlink or pyocd
FLASH_PROGRM	?= pyocd

##### Toolchains #######

ARM_TOOCHAIN	?= /opt/gcc-arm/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin

# path to JLinkExe
JLINKEXE		?= /opt/SEGGER/JLink/JLinkExe
# JLink device type, options: PY32F003X4, PY32F003X6, PY32F003X8, PY32F030X6, PY32F030X7, PY32F030X8
JLINK_DEVICE	?= PY32F003X8
# path to PyOCD
PYOCD_EXE		?= pyocd
# PyOCD device type, options: py32f003x4, py32f003x6, py32f003x8, py32f030x3, py32f030x4, py32f030x6, py32f030x7, py32f030x8
PYOCD_DEVICE	?= py32f003x8


##### Paths ############

# Link descript file: py32f003x6.ld, py32f003x8.ld, py32f030x6.ld, py32f030x8.ld
LDSCRIPT		= Libraries/LDScripts/py32f003x8.ld
# Library build flags: PY32F030x3, PY32F030x4, PY32F030x6, PY32F030x7, PY32F030x8, PY32F003x4, PY32F003x6, PY32F003x8
LIB_FLAGS       = PY32F003x8
```

## 5. Compiling And Flashing

```bash
# clean source code
make clean
# build
make
# or make with verbose output
V=1 make
# flash
make flash
```

# Debugging In VSCode

Install Cortex Debug extension, add a new configuration in launch.json, e.g.
```
{
    "armToolchainPath": "/opt/gcc-arm/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/",
    "toolchainPrefix": "arm-none-eabi",
    "name": "Cortex Debug",
    "cwd": "${workspaceFolder}",
    "executable": "${workspaceFolder}/Build/app.elf",
    "request": "launch",        // can be launch or attach
    "type": "cortex-debug",
    "runToEntryPoint": "Reset_Handler", // "main" or other function name. runToMain is deprecated
    "servertype": "jlink",  // jlink, openocd, pyocd, pe and stutil
    "device": "PY32F030X8",
    "interface": "swd",
    "preLaunchTask": "build",  // Set this to run a task from tasks.json before starting a debug session
    // "preLaunchCommands": ["Build all"], // Uncomment this if not using preLaunchTask
    "svdFile": "${workspaceFolder}/Misc/SVD/py32f030xx.svd",  // svd for this part number
    "showDevDebugOutput": "vscode", // parsed, raw, vscode:vscode log and raw
    "swoConfig":
    {
        "enabled": true,
        "cpuFrequency": 8000000, // Target CPU frequency in Hz
        "swoFrequency":  4000000,
        "source": "probe", // either be “probe” to get directly from the debug probe,
                           // or a serial port device to use a serial port external to the debug probe.
        "decoders":
        [
            {
                "label": "ITM port 0 output",
                "type": "console",
                "port": 0,
                "showOnStartup": true,
                "encoding": "ascii"
            }
        ]
    }
}
```
If Cortex Debug cannot find JLinkGDBServerCLExe, add the following line to settings.json
```
"cortex-debug.JLinkGDBServerPath": "/opt/SEGGER/JLink/JLinkGDBServerCLExe",
```

# Try Other Examples

More examples can be found in *Examples* folder, copy and replace the files under *User* folder to try different examples.

# Links

* Puya Product Page(Datasheet & SDK download): https://www.puyasemi.com/cpzx3/info_267_aid_242_kid_235.html
