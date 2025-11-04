# IWR6843AOP ESPHome Flash Integration

## 📋 Übersicht

Diese Integration ermöglicht das Over-The-Air (OTA) Flashen des IWR6843AOP mmWave Radar-Sensors direkt über ESPHome, ohne externe Tools wie UniFlash oder Python-Skripte.

## 🔧 Hardware-Setup

### Verbindungen

```
ESP32          →  IWR6843AOP
---------------------------------
GPIO3 (TX)     →  UART0_RX (Pin 4)
GPIO1 (RX)     ←  UART0_TX (Pin 5)
GPIO23 (RX)    ←  UART1_TX (Pin 13)
GPIO22         →  RESET (nRST)
GND            →  GND
```

### Flash-Modus (SOP Jumper)

Bevor du den "Vital Button" drückst:

1. **SOP0** = HIGH (3.3V oder offen)
2. **SOP1** = HIGH (3.3V oder offen)  
3. **SOP2** = LOW (GND)

⚠️ **WICHTIG**: Die SOP-Pins müssen **VOR** dem Drücken des Vital Button korrekt gesetzt werden!

## 📂 Projekt-Struktur

```
/
├── iwr6843_with_flash.yaml       # Haupt-ESPHome Konfiguration
├── components/
│   ├── iwr6843_flasher/          # Flash Component (Optional - Vollständig)
│   │   ├── __init__.py
│   │   ├── iwr6843_flasher.h
│   │   └── iwr6843_flasher.cpp
│   └── iwr6843_tracker/          # Tracking Component (Deine existierende)
├── firmware/
│   └── vital_signs_tracking_6843AOP_demo.bin
└── flash_iwr6843aop.py           # Standalone Python Tool (Backup)
```

## 🚀 Verwendung

### Methode 1: Über ESPHome WebUI

1. **SOP Jumper setzen** (siehe oben)
2. Navigiere zur ESPHome WebUI (`http://iwr6843.local`)
3. Drücke den **"Vital Button"**
4. Überwache den Fortschritt in den Logs
5. Nach erfolgreichem Flash: **SOP Jumper zurücksetzen** auf Normal-Modus
6. Device neustarten

### Methode 2: Über Home Assistant

1. **SOP Jumper setzen**
2. In Home Assistant: Drücke den Button-Entity: `button.iwr6843_vital_button`
3. Überwache Status: `sensor.iwr6843_flash_status` und `sensor.iwr6843_flash_progress`
4. Nach Flash: **SOP Jumper zurücksetzen** und Gerät neustarten

### Methode 3: Über ESPHome API

```python
import asyncio
from aioesphomeapi import APIClient

async def flash_iwr6843():
    client = APIClient("iwr6843.local", 6053, "API_PASSWORD")
    await client.connect()
    
    # Trigger Vital Button
    await client.button_command("vital_flash_button")
    
    await client.disconnect()

asyncio.run(flash_iwr6843())
```

## 📥 Firmware von GitHub einbetten

### Option A: Zur Compile-Zeit (Empfohlen)

Füge in `iwr6843_with_flash.yaml` hinzu:

```yaml
# Download firmware at compile time
external_files:
  - id: vital_firmware
    file: https://raw.githubusercontent.com/YOUR_USERNAME/YOUR_REPO/main/firmware/vital_signs_tracking_6843AOP_demo.bin
    type: binary
```

Dann im Lambda:

```yaml
- lambda: |-
    // Get embedded firmware
    extern const uint8_t vital_firmware_start[] asm("_binary_vital_firmware_start");
    extern const uint8_t vital_firmware_end[] asm("_binary_vital_firmware_end");
    size_t firmware_size = vital_firmware_end - vital_firmware_start;
    
    std::vector<uint8_t> firmware_data(vital_firmware_start, vital_firmware_end);
    
    ESP_LOGI("flash", "Firmware loaded: %zu bytes", firmware_size);
    // Flash procedure continues...
```

### Option B: Lokale Datei

1. Lege die Firmware-Datei im Projekt-Ordner ab
2. Verwende `file://` URL:

```yaml
substitutions:
  firmware_github_url: "file://firmware/vital_signs_tracking_6843AOP_demo.bin"
```

## 🔍 Debugging

### Logs überwachen

```bash
# ESPHome Logs in Echtzeit
esphome logs iwr6843_with_flash.yaml

# Oder über WebUI
http://iwr6843.local/logs
```

### Typische Log-Ausgabe (Erfolg)

```
[I][flash:123] 🩺 === VITAL SIGNS FIRMWARE FLASH START ===
[I][flash:124] 📍 Schritt 1/6: Reset IWR6843...
[I][flash:125] 🔄 RESET: Ziehe GPIO22 für 100ms auf LOW...
[I][flash:126] ✅ RESET: GPIO22 ist wieder HIGH.
[I][flash:127] 📍 Schritt 2/6: Sende UART Break Signal...
[I][flash:128] ⚡ Sending UART break signal...
[I][flash:129] ✅ UART break complete!
[I][flash:130] 📍 Schritt 3/6: Verbinde mit Bootloader...
[I][flash:131] Bootloader Response empfangen: 5 Bytes
[I][flash:132] ✅ Bootloader ACK empfangen!
[I][flash:133] 📍 Schritt 4/6: Flashe Firmware...
[I][flash:134] Progress: 10/100 chunks (10%)
[I][flash:135] Progress: 50/100 chunks (50%)
[I][flash:136] Progress: 100/100 chunks (100%)
[I][flash:137] ✅ File flashed successfully to SFLASH
[I][flash:138] 🎉 === VITAL FIRMWARE FLASH COMPLETE ===
```

### Fehlerbehandlung

#### Kein Bootloader ACK

```
[W][flash:140] ⚠️ Keine Antwort vom Bootloader (Timeout)
```

**Lösung:**
- Prüfe SOP Jumper Einstellung
- Stelle sicher, dass Reset-Signal korrekt ist
- Prüfe UART-Verkabelung (TX ↔ RX)

#### Checksum Error

```
[W][flash:141] Checksum mismatch: calculated 0xAB, received 0xCD
```

**Lösung:**
- UART-Baudrate prüfen (muss 115200 sein)
- Kabelqualität prüfen (kurze Kabel verwenden)
- GND-Verbindung sicherstellen

#### Flash Permission Denied

```
[E][flash:142] Erase command not acknowledged
```

**Lösung:**
- SOP Jumper **vor** Flash-Vorgang setzen
- Device vollständig zurücksetzen
- Prüfe ob Device im Bootloader-Modus ist

## 🔐 Sicherheit

### Production Mode

Für Produktions-Umgebungen:

```yaml
api:
  encryption:
    key: "DEIN_SICHERER_KEY"
  services:
    # Erlaube Flash nur für autorisierte Benutzer
    - service: flash_vital_firmware
      then:
        - if:
            condition:
              # Füge hier Authentifizierung hinzu
              lambda: 'return true;'
            then:
              - button.press: vital_flash_button
```

## 📊 Status-Monitoring

### Sensors

- `sensor.iwr6843_flash_progress` - Fortschritt 0-100%
- `text_sensor.iwr6843_flash_status` - Status-Text
- `binary_sensor.iwr6843_flash_active` - Flash läuft (true/false)

### Automation Beispiel (Home Assistant)

```yaml
automation:
  - alias: "IWR6843 Flash Benachrichtigung"
    trigger:
      - platform: state
        entity_id: text_sensor.iwr6843_flash_status
        to: "Flash complete!"
    action:
      - service: notify.mobile_app
        data:
          title: "IWR6843 Flash"
          message: "Vital Signs Firmware erfolgreich geflasht!"
```

## 🐛 Bekannte Einschränkungen

1. **Firmware-Größe**: Max. ~1MB durch ESP32 RAM-Limitierung
2. **Flash-Zeit**: Ca. 2-3 Minuten für 500KB Firmware
3. **UART-Geschwindigkeit**: Fixed auf 115200 Baud (Bootloader-Limit)
4. **Keine Verify**: Aktuell keine automatische Verifikation nach Flash

## 🔧 Erweiterte Konfiguration

### Custom Flash Script

```yaml
script:
  - id: custom_flash_sequence
    then:
      # Deine eigene Flash-Sequenz
      - logger.log: "Custom flash..."
```

### Multiple Firmware Files

```yaml
button:
  - platform: template
    name: "Flash Demo Firmware"
    on_press:
      - script.execute: flash_demo_firmware
  
  - platform: template
    name: "Flash Production Firmware"
    on_press:
      - script.execute: flash_production_firmware
```

## 📞 Support

- **Issues**: GitHub Issues
- **Docs**: [TI IWR6843AOP User Guide](https://www.ti.com/product/IWR6843)
- **ESPHome**: [ESPHome Documentation](https://esphome.io/)

## 📜 Lizenz

Basierend auf TI mmWave SDK Tools (Texas Instruments)

---

**⚠️ WICHTIG**: Dieses Tool ist für Entwicklung und Testing. Für Production-Umgebungen empfehlen wir zusätzliche Sicherheitsmaßnahmen und Fehlerbehandlung.

