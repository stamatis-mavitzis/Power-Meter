# Arduino Power Meter

A compact power measurement and data-logging system based on an **Arduino Pro Mini** and the **INA219 current/voltage sensor**. The system measures voltage, current, power, and accumulated energy, displays the measurements on an OLED screen, and stores the data on a microSD card in CSV format.

## Features

- Voltage measurement using the INA219
- Current measurement using the INA219
- Real-time power calculation
- Energy accumulation in mWh
- OLED display for live measurements
- CSV data logging to a microSD card
- Measurement interval of **100 ms**
- Status LEDs for system state and measurement errors
- Suitable for long-duration power-consumption and endurance tests

## Hardware

### Main Components

- Arduino Pro Mini
- INA219 voltage/current sensor
- SSD1306 OLED display
- microSD card module
- microSD card
- 3 × LEDs
- Current-limiting resistors
- External power supply / device under test

### Pin Configuration

| Component | Signal | Arduino Pro Mini |
|---|---|---|
| INA219 | SDA | A0 |
| INA219 | SCL | A1 |
| OLED | SDA | A0 |
| OLED | SCL | A1 |
| SD Card | MISO | D13 |
| SD Card | MOSI | D12 |
| SD Card | SCK | D11 |
| SD Card | CS | D10 |
| LED1 – SYSTEM OK | Output | D2 |
| LED2 – SUCCESSFUL MEASUREMENT | Output | D3 |
| LED3 – ERROR | Output | D4 |

The INA219 and OLED share the same I²C bus.

## Measurement

The power meter samples the device under test every **100 ms**.

For each measurement:

- Voltage is measured in volts (V)
- Current is measured in milliamperes (mA)
- Power is calculated in milliwatts (mW)
- Energy is accumulated in milliwatt-hours (mWh)

Power is calculated as:

\[
P = V \times I
\]

where:

- \(P\) = power in mW
- \(V\) = voltage in V
- \(I\) = current in mA

Energy is calculated by integrating the measured power over time:

\[
E = \sum P_i \Delta t_i
\]

with the time interval converted to hours.

## CSV Data Format

Measurements are stored on the SD card as CSV files.

The data format is:

```text
Time_ms,Voltage_V,Current_mA,Power_mW,Energy_mWh
```

Example:

```text
100,3.301,67.42,222.49,0.00618
200,3.300,67.51,222.78,0.01236
300,3.299,67.48,222.57,0.01855
```

The `Time_ms` column represents elapsed measurement time, while `Energy_mWh` represents accumulated energy.

## OLED Display

The OLED provides real-time information about the measurement, including values such as:

- Voltage
- Current
- Power
- Accumulated energy

The display allows the system to be monitored without connecting it to a computer.

## Status LEDs

| LED | Function |
|---|---|
| LED1 | System OK |
| LED2 | Successful measurement / data logging |
| LED3 | Error |

These indicators provide a simple visual indication of the logger's operating state.

## Software

The firmware is written for the **Arduino Pro Mini** using the Arduino development environment.

Main libraries used:

- `INA219` library for voltage/current measurement
- `Adafruit SSD1306` for the OLED display
- `Adafruit GFX` for graphics
- `SdFat` for microSD card access

Install the required libraries through the Arduino IDE Library Manager or add them manually to the Arduino libraries directory.

## Measurement Workflow

```text
          Device Under Test
                 │
                 ▼
             ┌───────┐
             │ INA219│
             └───┬───┘
                 │
          Voltage / Current
                 │
                 ▼
          Arduino Pro Mini
            │          │
            │          │
            ▼          ▼
        OLED Display  SD Card
                         │
                         ▼
                      CSV File
```

The Arduino periodically reads the INA219, calculates power and accumulated energy, updates the OLED, and writes the measurement to the SD card.

## Long-Duration Measurements

The power meter can be used for extended measurements such as:

- Power-consumption characterization
- Battery-powered system testing
- 24-hour endurance tests
- Energy-harvesting experiments
- Comparison of different operating modes
- Average and peak power measurements

For long measurements, the CSV file can later be processed using a Python script to calculate quantities such as:

- Average voltage
- Average current
- Average power
- Maximum power
- Total measured energy
- Energy per hour
- Measurement duration

## Data Analysis

The recorded CSV data can be processed externally using Python or another data-analysis environment.

The recorded quantities can be used to determine the average power:

$$
P_{\mathrm{avg}} = \frac{1}{N}\sum_{i=1}^{N} P_i
$$

and the total energy consumed during the measurement:

$$
E_{\mathrm{total}} = \sum_{i=1}^{N} P_i \Delta t_i
$$

This makes the power meter suitable for quantitative analysis of low-power electronic systems.

## Repository Structure

A typical repository structure is:

```text
power-meter/
├── firmware/
│   └── power_meter.ino
├── data/
│   └── example.csv
├── analysis/
│   └── python_scripts/
└── README.md
```

## Applications

This power meter was developed as a general-purpose instrument for measuring the electrical consumption and energy usage of embedded and low-power electronic systems.

It is particularly useful for evaluating systems where both **instantaneous power** and **energy consumption over time** are important.

## License

This project is intended for educational, research, and experimental use.

If you plan to redistribute or commercially use the firmware or hardware design, check the licenses of the Arduino libraries and other third-party components used by the project.
