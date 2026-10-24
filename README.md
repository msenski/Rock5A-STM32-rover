# Rock5A + STM32 Rover

A small 4-wheel robotic rover steered by a STM32 MCU (Nucleo G431KB) and a Rock5A 8GB.

## Architecture

The Rock 5A acts as the high-level computer, while the STM32 handles low-level hardware control.


```mermaid
flowchart LR
    UI["Phone / Laptop<br/>Web UI"] <-->|Wi-Fi / HTTP| ROCK["Rock 5A<br/>High-level control"]

    ROCK <-->|UART| STM["STM32 NUCLEO-G431KB<br/>Low-level control"]

    subgraph SENSORS["Sensors"]
        IMU["MPU6050 / GY-521<br/>Accel + Gyro"]
        TOF["VL53L1X<br/>Distance Sensor"]
    end

    subgraph ACTUATION["Actuation"]
        DRV["DRV8833"]
        MOTORS["4x DC Motors / Wheels"]
    end

    subgraph DEBUG["Debug / Bring-up"]
        USBTTL["USB-to-TTL Converter"]
    end

    IMU <-->|I2C| STM
    TOF <-->|I2C| STM

    STM -->|PWM + GPIO| DRV
    DRV --> MOTORS

    BAT["Battery Pack"] --> DRV

    USBTTL <-.-> ROCK
    USBTTL <-.-> STM
```


The goal is to gradually extend the system with sensors, feedback, and eventually higher-level autonomous behavior.

## Folder structure

```text
.
├── firmware/
│   └── rover-controller/    # STM32 (Nucleo G431KB) firmware project
│       ├── Core/            # Application source and header files
│       ├── Drivers/         # STM32 HAL, CMSIS, and board-support libraries
│       ├── cmake/           # ARM toolchain and STM32CubeMX CMake support
│       └── rover-controller.ioc  # STM32CubeMX configuration
├── software/
│   └── rover-gateway/       # Rock 5A C++ gateway application
│       ├── include/         # Public C++ headers
│       └── src/             # Application and serial-port implementation
└── .vscode/                 # Workspace editor settings
```

The `firmware` project controls the rover hardware and is generated/configured with STM32CubeMX. The `software` project runs on the Rock 5A and provides the high-level side of the UART connection to the STM32.

## Hardware

- **Rock 5A (8 GB)**: Linux (Ubuntu) SBC for high-level control [link](https://docs.radxa.com/en/rock5/rock5a/getting-started/introduction)
- **STM32 NUCLEO-G431KB**: microcontroller for real-time motor control [link](https://www.st.com/en/evaluation-tools/nucleo-g431kb.html#documentation) 
- **4-wheel robot chassis**: 4 DC geared motors, wheels, mounting plates [link](https://amzn.eu/d/0azrdLgF)
- **DRV8833**:  dual H-bridge motor driver [link](https://amzn.eu/d/0gSeg3x7)
- **L298N**:  alternative motor drivers (as a backup) [link](https://amzn.eu/d/02MSqJXz)
- **MPU6050** (GY-521):  accelerometer + gyroscope [link](https://amzn.eu/d/01muZGfY)
- **VL53L1X**:  laser distance sensor for obstacle detection [link](https://amzn.eu/d/02bBO0LR)
- **USB to TTL Serial Module**:  UART to USB converter [link](https://amzn.eu/d/01C2FUlQ)

## Milestones

- [x] Set up Rock 5A with Linux and remote development
- [x] Set up STM32 firmware and UART communication
- [x] Establish UART communication between Rock 5A and STM32
- [x] Implement a C++ serial port abstraction on Rock 5A
- [ ] Generate PWM signals with STM32 timers
- [ ] Control one DC motor through DRV8833
- [ ] Control all four wheels (forward, reverse, turning)
- [ ] Send motor commands from Rock 5A over UART
- [ ] Add sensors and feedback
- [ ] Implement basic autonomous behavior
