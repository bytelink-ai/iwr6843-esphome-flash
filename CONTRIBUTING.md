# Contributing to IWR6843 ESPHome Flash

Vielen Dank für dein Interesse, zu diesem Projekt beizutragen! 🎉

## 🤝 Wie kann ich beitragen?

### Bug Reports 🐛

Wenn du einen Bug findest:

1. **Prüfe**, ob der Bug bereits in den [Issues](https://github.com/bytelink-ai/iwr6843-esphome-flash/issues) gemeldet wurde
2. **Erstelle** ein neues Issue mit:
   - Klarem, beschreibendem Titel
   - Detaillierter Beschreibung des Problems
   - Schritten zur Reproduktion
   - Erwartetes vs. tatsächliches Verhalten
   - ESPHome-Version, Hardware-Info
   - Logs/Screenshots wenn möglich

### Feature Requests 💡

Hast du eine Idee für ein neues Feature?

1. **Öffne** ein Issue mit dem Tag `enhancement`
2. **Beschreibe** das Feature detailliert:
   - Was soll es tun?
   - Warum ist es nützlich?
   - Wie könnte es implementiert werden?

### Code Contributions 💻

#### Setup Development Environment

```bash
# Repository forken und klonen
git clone https://github.com/DEIN_USERNAME/iwr6843-esphome-flash.git
cd iwr6843-esphome-flash

# Development Dependencies installieren
pip install -r requirements.txt
pip install pre-commit black pylint

# Pre-commit Hooks aktivieren
pre-commit install
```

#### Development Workflow

1. **Branch erstellen**
   ```bash
   git checkout -b feature/dein-feature-name
   # oder
   git checkout -b fix/bug-beschreibung
   ```

2. **Code ändern**
   - Folge dem bestehenden Code-Stil
   - Kommentiere komplexe Logik
   - Schreibe aussagekräftige Commit-Messages

3. **Testen**
   ```bash
   # ESPHome Config validieren
   esphome config iwr6843_with_flash.yaml
   
   # Kompilieren
   esphome compile iwr6843_with_flash.yaml
   ```

4. **Commit**
   ```bash
   git add .
   git commit -m "feat: Deine Feature-Beschreibung"
   ```
   
   Commit-Message Format:
   - `feat:` - Neues Feature
   - `fix:` - Bug Fix
   - `docs:` - Dokumentation
   - `style:` - Code-Style (Formatierung)
   - `refactor:` - Code-Refactoring
   - `test:` - Tests hinzufügen
   - `chore:` - Maintenance

5. **Push & Pull Request**
   ```bash
   git push origin feature/dein-feature-name
   ```
   
   Dann öffne einen Pull Request auf GitHub mit:
   - Klarer Beschreibung der Änderungen
   - Link zum zugehörigen Issue (falls vorhanden)
   - Screenshots/Videos bei UI-Änderungen

## 📋 Code Guidelines

### Python Code

```python
# Guter Code-Stil
def download_firmware(url: str, destination: Path) -> bool:
    """
    Download firmware from URL to destination.
    
    Args:
        url: GitHub raw URL to firmware file
        destination: Local path to save firmware
        
    Returns:
        True if download successful, False otherwise
    """
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        # ...
        return True
    except RequestException as e:
        logger.error(f"Download failed: {e}")
        return False
```

### ESPHome YAML

```yaml
# Klare Struktur mit Kommentaren
sensor:
  # Flash Progress Indicator
  - platform: template
    id: flash_progress
    name: "Flash Progress"
    unit_of_measurement: "%"
    accuracy_decimals: 0
```

### C++ Component Code

```cpp
// Header Guards
#pragma once

// Includes
#include "esphome/core/component.h"

// Namespace
namespace esphome {
namespace iwr6843_flasher {

// Gute Dokumentation
/**
 * @brief IWR6843 Flasher Component
 * 
 * Implements TI mmWave bootloader protocol for
 * over-the-air firmware updates.
 */
class IWR6843Flasher : public Component {
  // ...
};

}  // namespace iwr6843_flasher
}  // namespace esphome
```

## 🧪 Testing

Bevor du einen Pull Request erstellst:

### Lokales Testen

1. **Config Validation**
   ```bash
   esphome config iwr6843_with_flash.yaml
   ```

2. **Compilation**
   ```bash
   esphome compile iwr6843_with_flash.yaml
   ```

3. **Hardware Test** (falls möglich)
   - Flash auf echte Hardware
   - Teste alle geänderten Features
   - Prüfe Logs auf Fehler

### CI/CD

Pull Requests triggern automatisch:
- ESPHome Config Validation
- Build Test
- Linting

## 📝 Dokumentation

Bei Code-Änderungen bitte auch Dokumentation aktualisieren:

- **README.md** - Hauptdokumentation
- **QUICKSTART.md** - Quick Start Guide
- **FLASH_README.md** - Detaillierte Flash-Dokumentation
- **Code Comments** - Inline-Dokumentation

## 🔍 Code Review Process

1. **Automated Checks** müssen bestehen (CI/CD)
2. **Review** von Maintainer(n)
3. **Feedback** wird als Comments hinterlassen
4. **Änderungen** durchführen falls nötig
5. **Merge** nach Approval

## 🎯 Gute erste Issues

Suche nach Issues mit dem Label `good first issue`:

- [Good First Issues](https://github.com/bytelink-ai/iwr6843-esphome-flash/labels/good%20first%20issue)

## 💬 Fragen?

- **GitHub Discussions**: [Discussions](https://github.com/bytelink-ai/iwr6843-esphome-flash/discussions)
- **Issue Tracker**: [Issues](https://github.com/bytelink-ai/iwr6843-esphome-flash/issues)

## 📜 Code of Conduct

Sei freundlich und respektvoll. Wir wollen eine positive Community.

## 🙏 Danke!

Jeder Beitrag zählt - ob Bug Report, Feature Request, Code oder Dokumentation!

---

**Happy Coding! 🚀**

