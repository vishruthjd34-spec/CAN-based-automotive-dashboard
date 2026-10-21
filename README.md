# 🚗 CAN-Based Automotive Dashboard

A **three-ECU automotive dashboard** built with **Embedded C** and the **CAN protocol**. Two sensor ECUs read vehicle data and broadcast it over the CAN bus; a display ECU receives everything and shows it live on a 16×2 character LCD.

![Language](https://img.shields.io/badge/Language-Embedded%20C-blue)
![MCU](https://img.shields.io/badge/MCU-PIC18F4580-orange)
![Protocol](https://img.shields.io/badge/Protocol-CAN-green)
![IDE](https://img.shields.io/badge/IDE-MPLAB%20X%20%2B%20XC8-lightgrey)

---

## 📖 About

Modern vehicles are made of many small computers called **ECUs** (Electronic Control Units) that talk to each other over a shared **CAN bus**. This project models that idea on a small scale:

- **ECU1** measures **vehicle speed** and detects the **gear position**
- **ECU2** measures **engine RPM** and **engine temperature**, and reads the **turn indicator** switches
- **ECU3** listens to the bus and shows all values on an **LCD dashboard**, and drives the **indicator LEDs**

It was built to practise **embedded C, CAN communication, ADC, digital input handling, character LCD interfacing and multi-ECU system design**.

---

## 🏗️ System Architecture

```
 ┌──────────────────────┐                                   ┌──────────────────────────┐
 │  ECU1 : Speed & Gear │                                   │   ECU3 : Display ECU     │
 │                      │                                   │                          │
 │  Potentiometer (AN4) │──► Speed ─┐                       │  16x2 CLCD               │
 │  Keypad (RC0-RC3)    │──► Gear  ─┤                       │   SPD  GR  RPM  TEMP     │
 └──────────────────────┘           │                       │                          │
                                    ▼                       │  Indicator LEDs          │
                          ═══════════════════ CAN BUS ═════►│   RB0/RB1  left          │
                                    ▲                       │   RB6/RB7  right         │
 ┌──────────────────────┐           │                       └──────────────────────────┘
 │ ECU2 : Engine & Ind. │           │
 │                      │──► RPM ───┤
 │  Potentiometer (AN4) │──► Temp ──┤
 │  Potentiometer (AN6) │──► Ind. ──┘
 │  Keypad (RC0-RC2)    │
 └──────────────────────┘
```

---

## ✨ Features

| | Feature | Description |
|---|---|---|
| 🏎️ | **Speed monitoring** | ADC value converted to a speed of 0–100 |
| ⚙️ | **Gear detection** | Gear up / down / neutral / reverse using a 4-switch keypad (gears 1–5) |
| 📈 | **Engine RPM** | ADC value converted to 0–6000 RPM |
| 🌡️ | **Engine temperature** | ADC value converted to 0–100 |
| ↔️ | **Turn indicators** | Left / right / off selected with switches, shown as LEDs on the display ECU |
| 📡 | **CAN communication** | Standard 11-bit identifiers, one message per parameter |
| 🖥️ | **LCD dashboard** | All values refreshed on a 16×2 LCD each time a message arrives |
| 🧩 | **Modular drivers** | Separate ADC, CAN, keypad, UART and CLCD drivers |

---

## 📡 CAN Message Map

All messages use **standard 11-bit IDs** (defined in `msg_id.h`):

| Message | ID | Sender | Length | Data format |
|---|---|---|---|---|
| 🏎️ Speed | `0x10` | ECU1 | 1 byte | `0 – 100` |
| ⚙️ Gear | `0x20` | ECU1 | 1 byte | `0` = Neutral, `1–5` = gear, `0xFF` = Reverse |
| 📈 RPM | `0x30` | ECU2 | 2 bytes | 16-bit value, **low byte first** (`0 – 6000`) |
| 🌡️ Engine temp | `0x40` | ECU2 | 1 byte | `0 – 100` |
| ↔️ Indicator | `0x50` | ECU2 | 1 byte | `0` = Off, `1` = Left, `2` = Right |

### 🔢 How the values are calculated

The 10-bit ADC gives a number from 0 to 1023, which is scaled to a real-world range:

```
speed = ADC × 100  / 1023      (ECU1, ADC channel 4)
rpm   = ADC × 6000 / 1023      (ECU2, ADC channel 4)
temp  = ADC × 100  / 1023      (ECU2, ADC channel 6)
```

### ⏱️ CAN bit timing

All three ECUs use identical bit timing (`BRGCON1 = 0xE1`, `BRGCON2 = 0x1B`, `BRGCON3 = 0x03`). At a 20 MHz clock this works out to about **22.6 kbit/s** (13 time quanta per bit). Every node on the bus **must** use the same values.

---

## 🎮 Controls

### ECU1 – Gear selection (keypad on `PORTC`, active low)

| Switch | Action |
|---|---|
| `SWITCH1` | Gear **up** (max gear 5) |
| `SWITCH2` | Gear **down** (down to neutral) |
| `SWITCH3` | **Neutral** |
| `SWITCH4` | **Reverse** |

> While in reverse, gear up / down are ignored — press Neutral first.

### ECU2 – Turn indicators

| Switch | Action |
|---|---|
| `SWITCH1` | **Left** indicator on |
| `SWITCH2` | **Right** indicator on |
| `SWITCH3` | Indicators **off** |

---

## 🖥️ LCD Layout

```
┌────────────────┐
│SPD GR RPM TEMP │
│045 G3 2850  072│
└────────────────┘
```

| Column | Content |
|---|---|
| 0 – 2 | Speed |
| 4 – 5 | Gear: `N` neutral, `G1`–`G5`, `R` reverse |
| 7 – 10 | Engine RPM |
| 13 – 15 | Engine temperature |

Indicator LEDs on ECU3: **left** = `RB0`, `RB1` — **right** = `RB6`, `RB7`.

---

## 🗂️ Project Structure

Each ECU is its own MPLAB X project folder:

```
CAN_Automotive_Dashboard/
├── ECU1_Speed_Gear/
│   ├── ecu1_main.c          # 🚪 Main loop: read speed + gear, transmit over CAN
│   ├── ecu1_sensor.c / .h   # 🏎️ get_speed(), get_gear_pos() with gear state logic
│   ├── adc.c / .h           # 📈 ADC driver
│   ├── digital_keypad.c / .h# 🔘 Keypad driver (level / state-change detection)
│   ├── can.c / .h           # 📡 CAN driver (init, transmit, receive)
│   ├── uart.c / .h          # 🔌 UART driver (for debugging)
│   └── msg_id.h             # 🆔 CAN message IDs
│
├── ECU2_Engine_Indicators/
│   ├── ecu2_main.c          # 🚪 Main loop: RPM, indicator, temperature -> CAN
│   ├── ecu2_sensor.c / .h   # ⚙️ get_rpm(), process_indicator()
│   ├── adc.c / .h  digital_keypad.c / .h  can.c / .h  uart.c / .h  msg_id.h
│
├── ECU3_Display/
│   ├── main.c               # 🚪 Init + receive loop
│   ├── message_handler.c / .h  # 📥 Decode CAN frames, update LCD and LEDs
│   ├── clcd.c / .h          # 🖥️ 16x2 character LCD driver
│   ├── can.c / .h           # 📡 CAN driver
│   └── msg_id.h             # 🆔 CAN message IDs
│
├── .gitignore
└── README.md
```

---

## 🔌 Hardware

| Part | Used for |
|---|---|
| CAN-capable **PIC18** microcontroller (PIC18F4580) × 3 | One per ECU, 20 MHz crystal |
| CAN transceiver × 3 (e.g. MCP2551) | Connects each ECU to the CAN bus (`RB2` = CANTX, `RB3` = CANRX) |
| Potentiometers | Speed / RPM (AN4) and engine temperature (AN6) |
| 4-switch digital keypad on `RC0`–`RC3` | Gear selection (ECU1) and indicators (ECU2) |
| 16×2 character LCD | Data on `PORTD`, `RS` = `RC0`, `RW` = `RC1`, `EN` = `RE2` (ECU3) |
| LEDs on `RB0`, `RB1`, `RB6`, `RB7` | Left / right indicators (ECU3) |
| 120 Ω resistors | CAN bus termination at both ends |

---

## 🚀 Getting Started

### ✅ Prerequisites

- **MPLAB X IDE**
- **XC8 compiler**
- A programmer such as PICkit

### 📥 Clone the repository

```bash
git clone https://github.com/<your-username>/<your-repo-name>.git
```

### 🔧 Build and flash (repeat for each ECU)

1. In MPLAB X choose **File → New Project → Standalone Project**.
2. Select the device **PIC18F4580** and the **XC8** compiler.
3. Add all the files from **one** ECU folder (`ECU1_…`, `ECU2_…` or `ECU3_…`) as source / header files.
4. Set the configuration bits (see below).
5. Build, then program the matching board.

### ⚙️ Configuration bits

`ECU3_Display/main.c` already contains:

```c
#pragma config OSC = HS, WDT = OFF, LVP = OFF, PBADEN = OFF
```

ECU1 and ECU2 do not have this line, so set the **same** configuration for them in MPLAB X (*Window → Target Memory Views → Configuration Bits*) or add the line to `ecu1_main.c` / `ecu2_main.c`.

### 🔗 Wiring the bus

Connect **CANH** to **CANH** and **CANL** to **CANL** on all three transceivers, put a **120 Ω** resistor across CANH / CANL at each end of the bus, and share a common ground.

---

## 🔄 How It Works

1. **ECU1** reads the speed potentiometer and the gear keypad, then sends the **Speed** and **Gear** frames every ~20 ms.
2. **ECU2** reads RPM, indicator switches and temperature, then sends the **RPM**, **Indicator** and **Temperature** frames every ~30 ms.
3. **ECU3** polls the CAN receive buffer. For each valid frame it matches the ID, stores the value, redraws the LCD and updates the indicator LEDs.

---

## 🧠 Concepts Used

- 📡 CAN protocol — standard frames, message IDs, bit timing, TX / RX buffers
- 📈 ADC — channel selection, acquisition time, scaling to real units
- 🔘 Digital input — state-change (edge) detection for keypad
- 🖥️ 16×2 character LCD interfacing in 8-bit mode
- 🧩 Modular driver design with separate `.c` / `.h` files
- 🔁 State handling for gear selection
- 🔌 UART driver (included for debugging)

---

## ⚠️ Known Limitations

- ECU3 **polls** the single CAN receive buffer every 20 ms while the other ECUs send a frame every ~10 ms, so some frames can be **missed**. Interrupt-driven reception or a faster polling loop would fix this
- When the gear changes from `G3` to `N` or `R`, the second LCD character is not cleared, so a leftover digit can remain
- `get_engine_temp()` is declared in `ecu2_sensor.h` but never defined (temperature is read directly with `read_adc()` in `ecu2_main.c`)
- The indicator macros in `ecu2_sensor.h` are unused — the LEDs are driven on ECU3
- `getch_with_timeout()` is declared in `uart.h` but not implemented, and UART is not used by the main programs
- Only `ECU3` sets the configuration bits in source

---

## 🔮 Future Improvements

- [ ] ⚡ Interrupt-driven CAN reception with message filters
- [ ] 🧯 CAN error handling and bus-off recovery
- [ ] 🕒 Blinking turn indicators and a hazard (both) mode
- [ ] 🔔 Add horn / door / fuel-level messages
- [ ] 🧹 Clear LCD characters correctly when the gear changes
- [ ] 🌡️ Real temperature sensor (e.g. LM35) with proper calibration

---

## 👨‍💻 Author

**Vishruth J D**

---

⭐ If you found this project helpful, consider giving it a star on GitHub!
