# 🎯 IWR6843AOP ESPHome Flash Integration

[![ESPHome](https://img.shields.io/badge/ESPHome-Compatible-blue.svg)](https://esphome.io/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![GitHub](https://img.shields.io/badge/GitHub-bytelink--ai-black.svg)](https://github.com/bytelink-ai/iwr6843-esphome-flash)

**Over-The-Air Firmware Flasher für Texas Instruments IWR6843AOP mmWave Radar Sensor via ESPHome**

Flash deinen IWR6843AOP Radar-Sensor direkt über ESPHome - ohne externe Tools, ohne Python-Skripte, einfach per Knopfdruck!

![Flash Demo](https://img.shields.io/badge/Status-Production%20Ready-brightgreen)

## ✨ Features

- 🩺 **One-Button Flash** - "Vital Button" für kompletten Flash-Vorgang
- 📡 **TI Bootloader-Protokoll** - Vollständige Implementation des mmWave Bootloader-Protokolls
- 📊 **Echtzeit-Monitoring** - Progress-Anzeige (0-100%) und Status-Updates
- 🔄 **OTA Updates** - Firmware-Updates über WiFi
- 👥 **5-Personen-Tracking** - Integrierte Radar-Tracking-Funktionalität
- 🚨 **Fall-Detection** - Automatische Sturz-Erkennung
- 🏠 **Home Assistant** - Native Integration
- 🛡️ **Fehlerbehandlung** - Robuste Error-Handling und Retry-Mechanismen

## 📋 Hardware-Anforderungen

- **ESP32** (empfohlen: ESP32-DevKit)
- **Texas Instruments IWR6843AOP** mmWave Radar Sensor
- **4 Verbindungskabel** (TX, RX, Reset, GND)
- **SOP Jumper** (für Flash-Modus)

## 🚀 Quick Start (5 Minuten)

### 1. Repository klonen

```bash
git clone https://github.com/bytelink-ai/iwr6843-esphome-flash.git
cd iwr6843-esphome-flash
```

### 2. Dependencies installieren

```bash
pip install -r requirements.txt
```

### 3. WiFi konfigurieren

Erstelle `secrets.yaml`:

```yaml
wifi_ssid: "Dein_WiFi_Name"
wifi_password: "Dein_WiFi_Passwort"
```

### 4. Firmware vorbereiten

**Option A - Von GitHub (empfohlen):**
```bash
python download_firmware.py
```

**Option B - Lokale Datei:**
Kopiere deine Firmware nach `firmware/vital_signs_tracking_6843AOP_demo.bin`

### 5. ESPHome kompilieren & hochladen

```bash
esphome run iwr6843_with_flash.yaml
```

### 6. Hardware verbinden

```
ESP32          →  IWR6843AOP
─────────────────────────────
GPIO3 (TX)     →  UART0_RX (Pin 4)
GPIO1 (RX)     ←  UART0_TX (Pin 5)
GPIO23 (RX)    ←  UART1_TX (Pin 13)
GPIO22         →  nRST (Reset)
GND            →  GND
```

### 7. Flash durchführen

1. **SOP Jumper setzen** (Flash-Modus):
   - SOP0 = HIGH
   - SOP1 = HIGH  
   - SOP2 = LOW (GND)

2. Öffne WebUI: `http://iwr6843.local`

3. Drücke den **"Vital Button"** 🩺

4. Warte ~2-3 Minuten

5. **Fertig!** ✅

## 📖 Dokumentation

- **[Quick Start Guide](QUICKSTART.md)** - Detaillierte Schritt-für-Schritt Anleitung
- **[Vollständige Dokumentation](FLASH_README.md)** - Alle Features und Konfigurationen
- **[Projekt-Übersicht](PROJECT_SUMMARY.md)** - Architektur und technische Details

## 🔧 Verwendung

### Via WebUI

```
http://iwr6843.local → Drücke "Vital Button"
```

### Via Home Assistant

```yaml
# Automation Beispiel
automation:
  - alias: "Flash IWR6843 bei Update"
    trigger:
      platform: state
      entity_id: binary_sensor.new_firmware_available
      to: "on"
    action:
      service: button.press
      target:
        entity_id: button.iwr6843_vital_button
```

### Via ESPHome API

```python
from aioesphomeapi import APIClient

client = APIClient("iwr6843.local", 6053, "API_PASSWORD")
await client.connect()
await client.button_command("vital_flash_button")
```

## 📊 Monitoring

Nach erfolgreichem Flash stehen folgende Sensoren zur Verfügung:

### Flash-Status
- `sensor.iwr6843_flash_progress` - Fortschritt (0-100%)
- `text_sensor.iwr6843_flash_status` - Status-Text

### Person-Tracking (1-5 Personen)
- `sensor.person_X_x/y/z` - 3D Position
- `sensor.person_X_velocity` - Geschwindigkeit
- `sensor.person_X_height` - Höhe
- `text_sensor.person_X_presence` - Anwesenheit
- `text_sensor.person_X_fall` - Sturz-Status

### Gesamt
- `sensor.total_people_count` - Anzahl erkannter Personen

## 🏗️ Architektur

```
┌─────────────────────────────────────┐
│        ESPHome (ESP32)              │
│                                     │
│  [Vital Button] → Flash Script     │
│       ↓                             │
│  1. Reset IWR6843                   │
│  2. Send UART Break Signal          │
│  3. Connect to Bootloader           │
│  4. Erase Flash (SFLASH)            │
│  5. Send Firmware (240B chunks)     │
│  6. Verify & Close                  │
└─────────────────────────────────────┘
              ↕ UART @ 115200
┌─────────────────────────────────────┐
│    IWR6843AOP mmWave Radar          │
│                                     │
│  → Bootloader Mode (Flash)          │
│  → Normal Mode (Tracking)           │
└─────────────────────────────────────┘
```

## 🔍 TI Bootloader-Protokoll

Vollständig implementiert:

- ✅ **Packet-Struktur**: SYNC (0xAA) + Length + Checksum + Data
- ✅ **Opcodes**: PING, GET_VERSION, ERASE, START_DOWNLOAD, SEND_DATA, FILE_CLOSE
- ✅ **ACK/NACK Handling**: Automatische Fehlerbehandlung
- ✅ **Chunk-Transfer**: 240 Bytes pro Packet
- ✅ **Checksum Verification**: Datenintegrität garantiert

## 🐛 Troubleshooting

### Problem: "No response from bootloader"

**Lösung:**
```
1. SOP Jumper prüfen (SOP2 muss auf GND!)
2. Reset-Button drücken
3. 2 Sekunden warten
4. "Vital Button" erneut drücken
```

### Problem: "UART timeout"

**Lösung:**
```
1. TX/RX Kabel prüfen (TX → RX, RX → TX)
2. GND-Verbindung sicherstellen
3. Baudrate prüfen (muss 115200 sein)
4. Kabelqualität (kurze Kabel verwenden)
```

### Problem: "Checksum error"

**Lösung:**
```
1. Bessere Kabel verwenden
2. GND-Verbindung verbessern
3. Störquellen entfernen
4. Baudrate reduzieren (falls nötig)
```

Mehr Troubleshooting: [FLASH_README.md#fehlerbehandlung](FLASH_README.md#-fehlerbehandlung)

## 📂 Projekt-Struktur

```
iwr6843-esphome-flash/
├── iwr6843_with_flash.yaml          # Haupt-ESPHome Konfiguration
├── components/
│   └── iwr6843_flasher/              # Custom Component (optional)
├── firmware/                         # Firmware-Dateien
│   └── vital_signs_tracking_6843AOP_demo.bin
├── user_files/
│   ├── configs/                      # CCXML Konfigurationen
│   ├── images/                       # Firmware-Images
│   └── settings/                     # Settings (COM Port etc.)
├── download_firmware.py              # Firmware Download Tool
├── flash_iwr6843aop.py              # Standalone Flash Tool (Backup)
├── QUICKSTART.md                     # Quick Start Guide
├── FLASH_README.md                   # Vollständige Dokumentation
└── PROJECT_SUMMARY.md                # Technische Übersicht
```

## 🛠️ Entwicklung

### Custom Component erweitern

```cpp
// components/iwr6843_flasher/iwr6843_flasher.cpp
bool IWR6843Flasher::custom_function() {
  // Deine Custom-Funktionalität
}
```

### Neue Firmware hinzufügen

```yaml
# iwr6843_with_flash.yaml
substitutions:
  custom_firmware_url: "https://github.com/USER/REPO/raw/main/firmware/custom.bin"
```

## 🤝 Contributing

Contributions sind willkommen! Bitte:

1. Fork das Repository
2. Erstelle einen Feature-Branch (`git checkout -b feature/AmazingFeature`)
3. Commit deine Änderungen (`git commit -m 'Add AmazingFeature'`)
4. Push zum Branch (`git push origin feature/AmazingFeature`)
5. Öffne einen Pull Request

## 📜 Lizenz

Dieses Projekt basiert auf Texas Instruments mmWave SDK Tools.

Siehe [LICENSE](LICENSE) für Details.

## 🙏 Credits

- **Texas Instruments** - mmWave Bootloader-Protokoll und SDK
- **ESPHome** - Home Automation Framework
- **Community Contributors** - Bug Reports und Features

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/bytelink-ai/iwr6843-esphome-flash/issues)
- **Discussions**: [GitHub Discussions](https://github.com/bytelink-ai/iwr6843-esphome-flash/discussions)
- **Documentation**: [Wiki](https://github.com/bytelink-ai/iwr6843-esphome-flash/wiki)

## 🌟 Star History

Wenn dir dieses Projekt hilft, gib ihm einen Star! ⭐

## 📈 Status

![Build Status](https://img.shields.io/github/actions/workflow/status/bytelink-ai/iwr6843-esphome-flash/esphome-build.yml)
![Last Commit](https://img.shields.io/github/last-commit/bytelink-ai/iwr6843-esphome-flash)
![Issues](https://img.shields.io/github/issues/bytelink-ai/iwr6843-esphome-flash)

---

**Made with ❤️ by [ByteLink AI](https://github.com/bytelink-ai)**

**Powered by ESPHome & Texas Instruments mmWave Technology**

