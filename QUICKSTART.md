# 🚀 IWR6843AOP ESPHome Flash - Quick Start

## 📦 Installation (5 Minuten)

### Schritt 1: Repository Setup

```bash
git clone https://github.com/YOUR_USERNAME/iwr6843-esphome-flash.git
cd iwr6843-esphome-flash
```

### Schritt 2: Python Dependencies

```bash
pip install -r requirements.txt
```

### Schritt 3: Secrets konfigurieren

Erstelle `secrets.yaml`:

```yaml
wifi_ssid: "Dein_WiFi_Name"
wifi_password: "Dein_WiFi_Passwort"
```

### Schritt 4: Firmware herunterladen

**Option A: Automatisch**
```bash
python download_firmware.py
```

**Option B: Manuell**
```bash
mkdir -p firmware
# Lade deine IWR6843AOP Firmware herunter und speichere sie als:
# firmware/vital_signs_tracking_6843AOP_demo.bin
```

### Schritt 5: GitHub URL anpassen

Bearbeite `iwr6843_with_flash.yaml`:

```yaml
substitutions:
  firmware_github_url: "https://raw.githubusercontent.com/DEIN_USERNAME/DEIN_REPO/main/firmware/vital_signs_tracking_6843AOP_demo.bin"
```

### Schritt 6: ESPHome kompilieren & hochladen

```bash
# Erstmalige Installation
esphome run iwr6843_with_flash.yaml

# Oder wähle Upload-Methode:
# - Serial (USB)
# - OTA (WiFi)
```

## 🔌 Hardware-Verbindung

### Minimale Verbindung für Flash:

```
ESP32 GPIO3  →  IWR6843 UART0_RX (Pin 4)
ESP32 GPIO1  ←  IWR6843 UART0_TX (Pin 5)
ESP32 GPIO22 →  IWR6843 nRST
ESP32 GND    →  IWR6843 GND
```

### Vollständige Verbindung (mit Tracking):

```
ESP32 GPIO3  →  IWR6843 UART0_RX (Pin 4)    # CLI
ESP32 GPIO1  ←  IWR6843 UART0_TX (Pin 5)    # CLI
ESP32 GPIO23 ←  IWR6843 UART1_TX (Pin 13)   # Data
ESP32 GPIO22 →  IWR6843 nRST                # Reset
ESP32 3.3V   →  IWR6843 VIN (3.3V)
ESP32 GND    →  IWR6843 GND
```

## 🎯 Firmware Flashen (3 Minuten)

### Vorbereitung

1. **SOP Jumper setzen** (Flash-Modus):
   - SOP0 = HIGH
   - SOP1 = HIGH
   - SOP2 = LOW (GND)

2. **Strom einschalten**

### Flash-Vorgang

**Via WebUI:**
1. Öffne `http://iwr6843.local`
2. Drücke **"Vital Button"**
3. Warte (~2 Minuten)
4. ✅ "Flash complete!" erscheint

**Via Home Assistant:**
1. Navigiere zu `Einstellungen → Geräte & Dienste → ESPHome`
2. Finde `IWR6843`
3. Drücke Button: `Vital Button`
4. Überwache Status in den Logs

**Via Terminal:**
```bash
# ESPHome API aufrufen
esphome logs iwr6843_with_flash.yaml

# In anderem Terminal:
curl -X POST http://iwr6843.local/button/vital_flash_button
```

### Nach dem Flash

1. **SOP Jumper zurücksetzen** (Normal-Modus):
   - SOP0 = LOW
   - SOP1 = LOW
   - SOP2 = LOW

2. **Gerät neustarten**:
   - Drücke Reset-Button
   - Oder: `curl -X POST http://iwr6843.local/button/device_reset_button`

3. **Sensor starten**:
   - Automatisch nach 2 Sekunden
   - Oder manuell: Drücke "Sensor Starten"

## 🔍 Logs überwachen

### Terminal:
```bash
esphome logs iwr6843_with_flash.yaml
```

### WebUI:
```
http://iwr6843.local/logs
```

### Home Assistant:
- Developer Tools → Logs
- Filter: `iwr6843`

## ✅ Erfolgs-Checkliste

- [ ] ESP32 mit IWR6843 verbunden
- [ ] WiFi konfiguriert (secrets.yaml)
- [ ] ESPHome hochgeladen
- [ ] WebUI erreichbar (`http://iwr6843.local`)
- [ ] SOP Jumper auf Flash-Modus
- [ ] "Vital Button" gedrückt
- [ ] Flash erfolgreich (2-3 Min)
- [ ] SOP Jumper zurück auf Normal
- [ ] Gerät neugestartet
- [ ] Sensor sendet Daten

## 🐛 Probleme?

### Problem: "No response from bootloader"

**Lösung:**
```
1. SOP Jumper prüfen (SOP2 muss GND sein!)
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
```

### Problem: "Firmware download failed"

**Lösung:**
```
1. GitHub URL prüfen
2. Internet-Verbindung des ESP32 testen
3. Firmware manuell herunterladen und lokal ablegen
```

### Problem: "Device offline after flash"

**Lösung:**
```
1. SOP Jumper zurücksetzen!
2. Power-Cycle (Strom aus/ein)
3. 30 Sekunden warten
4. WebUI neu laden
```

## 📊 Status-Überprüfung

### WebUI Sensors:

- **Flash Progress**: 0-100% Fortschritt
- **Flash Status**: Aktueller Status-Text
- **Person 1-5 X/Y/Z**: Tracking-Daten (nach Flash)

### Erwartete Werte nach erfolgreichem Flash:

```
Flash Status: "Bereit"
Flash Progress: 0%
Person 1-5: Aktive Koordinaten (wenn Personen erkannt)
```

## 🎉 Fertig!

Dein IWR6843AOP ist jetzt:
- ✅ Mit Vital Signs Firmware geflasht
- ✅ Über ESPHome steuerbar
- ✅ Bereit für Person-Tracking
- ✅ OTA-updatefähig

## 🔄 Firmware Update

Für zukünftige Updates:

1. Neue Firmware in GitHub Repository aktualisieren
2. ESPHome neu kompilieren: `esphome run iwr6843_with_flash.yaml`
3. OTA-Upload auf ESP32
4. "Vital Button" drücken
5. Fertig!

## 📚 Weiterführende Links

- [Vollständige Dokumentation](FLASH_README.md)
- [TI IWR6843AOP Datenblatt](https://www.ti.com/product/IWR6843)
- [ESPHome Docs](https://esphome.io/)
- [Troubleshooting Guide](FLASH_README.md#-fehlerbehandlung)

---

**💡 Tipp**: Speichere die URL zu deinem WebUI als Lesezeichen: `http://iwr6843.local`

