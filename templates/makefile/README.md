# [py32f0-template](https://github.com/IOsetting/py32f0-template)

[![Build Status](https://github.com/SoCXin/PY32F002/workflows/templates/badge.svg)](https://github.com/SoCXin/PY32F002/actions/workflows/templates.yml)

* Puya PY32F0 template project for GNU Arm Embedded Toolchain
* Supported programmers: J-Link, DAPLink/PyOCD
* Supported IDE: VSCode

There is high probability that PY32F002A, PY32F003 and PY32F030 share the same core, despite all the parts listed, you can treat them all as PY32F030 in coding and programming.

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
│   │   ├── JLinkDevices        # JLink flash loaders
│   │   └── Sources             # Flash algorithm source code
│   ├── Puya.PY32F0xx_DFP.x.pack # DFP pack file for PyOCD
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
sudo dpkg -i JLink_Linux_V786_x86_64.deb
```

The default installation directory is */opt/SEGGER*

Copy [Project directory]/Misc/Flash/JLinkDevices to [User home]/.config/SEGGER/JLinkDevices/
```bash
cd py32f0-template
cp -r Misc/Flash/JLinkDevices/ ~/.config/SEGGER/
```
Read more: [https://wiki.segger.com/J-Link_Device_Support_Kit](https://wiki.segger.com/J-Link_Device_Support_Kit)

## 2. Option #2: Install PyOCD

Don't install from apt repository, because the version 0.13.1+dfsg-1 is too low for J-Link probe.

Install PyOCD from pip

```bash
pip install pyocd
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
* **USE_LL_LIB** Puya provides two sets of library, HAL and LL, set `USE_LL_LIB ?= y` to use LL instead of HAL.
* **ENABLE_PRINTF_FLOAT** set it to `y` to `-u _printf_float` to link options. This will increase the binary size.
* **USE_FREERTOS** Set `USE_FREERTOS ?= y` will include FreeRTOS in compilation
* **USE_DSP** Include CMSIS DSP or not
* **FLASH_PROGRM**
  * If you use J-Link, `FLASH_PROGRM` can be jlink or pyocd
  * If you use DAPLink, set `FLASH_PROGRM ?= pyocd`
  * ST-LINK is not supported yet.
* **ARM_TOOCHAIN** Make sure it points to the correct path of arm-none-eabi-gcc

```makefile
##### Project #####

PROJECT           ?= app
# The path for generated files
BUILD_DIR         = Build


##### Options #####

# Use LL library instead of HAL, y:yes, n:no
USE_LL_LIB        ?= n
# Enable printf float %f support, y:yes, n:no
ENABLE_PRINTF_FLOAT ?= n
# Build with FreeRTOS, y:yes, n:no
USE_FREERTOS      ?= n
# Build with CMSIS DSP functions, y:yes, n:no
USE_DSP           ?= n
# Programmer, jlink or pyocd(DAPLink)
FLASH_PROGRM      ?= pyocd

##### Toolchains #######
ARM_TOOCHAIN      ?= /opt/gcc-arm/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin

# path to JLinkExe
JLINKEXE          ?= /opt/SEGGER/JLink/JLinkExe
# JLink device type, options:
#   PY32F002AX5, PY32F002X5, 
#   PY32F003X4, PY32F003X6, PY32F003X8, 
#   PY32F030X4, PY32F030X6, PY32F030X7, PY32F030X8
JLINK_DEVICE      ?= PY32F030X6
# path to PyOCD, 
PYOCD_EXE         ?= pyocd
# PyOCD device type, options: 
#     py32f002ax5, py32f002x5, 
#   py32f003x4,  py32f003x6, py32f003x8, 
#   py32f030x3,  py32f030x4, py32f030x6, py32f030x7, py32f030x8
#   py32f072xb
PYOCD_DEVICE      ?= py32f030x8


##### Paths ############

# Link descript file: py32f002x5.ld, py32f003x6.ld, py32f003x8.ld, py32f030x6.ld, py32f030x8.ld
LDSCRIPT          = Libraries/LDScripts/py32f030x8.ld
# Library build flags: 
#   PY32F002x5, PY32F002Ax5, 
#   PY32F003x4, PY32F003x6, PY32F003x8, 
#   PY32F030x3, PY32F030x4, PY32F030x6, PY32F030x7, PY32F030x8, 
#   PY32F072xB
LIB_FLAGS         = PY32F030x6
```

DAPLink调试问题
```
W [Errno 13] Access denied (insufficient permissions) while trying to interrogate a USB device (VID=0d28 PID=0204). This can probably be remedied with a udev rule. See <https://github.com/pyocd/pyOCD/tree/master/udev> for help. [pyusb_backend]
```
[修复方式](https://github.com/OS-Q/pyOCD/tree/main/udev)
```
$ sudo cp ../50-cmsis-dap.rules /etc/udev/rules.d
$ sudo udevadm control --reload
$ sudo udevadm trigger
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
