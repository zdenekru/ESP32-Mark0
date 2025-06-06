# ESP32 Mark0

**ESP32 Mark0** is a powerful and not-so-compact IoT development project based on the ESP32-WROOM-32 microcontroller. This project showcases both hardware and firmware design, with a focus on wireless communication, embedded control, and a simple web dashboard interface.

---

## 🚀 Project Overview

ESP32 Mark0 is designed to:

- Prototype embedded systems using Wi-Fi and basic web interface
- Interface with sensors and other potential peripherals
- Provide a physical platform for experimenting with ESP32 capabilities
- Demonstrate firmware/web integration between Arduino and Python/HTML

> Developed step-by-step with the help of OpenAI's ChatGPT — from schematic to code.

---

## 🔧 Features

- **ESP32-WROOM-32** module
- USB-C interface with **CH340** USB-to-Serial bridge
- Reset and Boot buttons
- Status LED
- Breadboard-compatible pinout
- Custom KiCad PCB (Rev0 and Rev1)
- Firmware written in **Arduino IDE**
- Python and HTML files edited in **VS Code**
- Manufactured via [JLCPCB.com](https://jlcpcb.com)

---

## 🗂️ Repository Structure

```
ESP32-Mark0/
├── 25Zak001 ESP32 Mark0-backups/ # Backup versions and old files
├── CodeESP32/ESP32_Mark0_Github_ready/ # Arduino firmware
├── CodePython/ # Python scripts and dashboard HTML
├── Manufacturing Files Rev0/ # Rev0 Gerbers and fab files
├── Manufacturing Files Rev1/ # Final Rev1 files for production
├── .kicad_ # KiCad project files (Rev0 and Rev1)
├── .gitignore
├── .gitattributes
├── README.md
├── fp-info-cache
├── qrCode.kicad_sym
└── 25Zak001 ESP32 Mark0.step # 3D model of the board
```

---

## 🛠️ Development Environment

### Prerequisites

- **Arduino IDE** with ESP32 board support
- USB-C cable
- **CH340** USB driver
- **KiCad** (for editing hardware files)
- **VS Code** (for editing HTML/Python dashboard files)

---

## ⚙️ Uploading Firmware

Upload the sketch via Arduino IDE after selecting the correct board and port:

- **Board**: ESP32 Dev Module
- **Upload Speed**: 115200
- **Port**: (as assigned by your system)

---

## 🌐 Web Dashboard

The web dashboard (in `CodePython/`) includes a lightweight HTML interface that interacts with the ESP32 over Wi-Fi. Customize this dashboard for sensor readouts, toggles, or control logic.

---

## 🏭 Manufacturing

Both **Rev0** and **Rev1** were designed in **KiCad** and manufactured via **[JLCPCB](https://jlcpcb.com)**. All Gerber, BOM, and position files are provided in the respective manufacturing folders.

---

## 🤖 AI-Assisted Engineering

This project was co-developed with **ChatGPT**, assisting in:

- ESP32 firmware debugging and optimization
- PCB design decisions and validation
- HTML/Python dashboard integration
- Repository structure and documentation

---

## 📜 License

MIT License – see `LICENSE` file.

---

Made with ☕, perseverance, and ChatGPT.


