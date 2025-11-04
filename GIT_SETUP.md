# 🚀 Git Repository Setup & Push Guide

## 📋 Status

Dein GitHub Repository ist bereit: https://github.com/bytelink-ai/iwr6843-esphome-flash.git

## 🎯 Schnell-Anleitung (Copy & Paste)

```bash
# 1. Navigiere zum Projekt-Ordner
cd "C:\Users\Eren\Downloads\IWR6843AOP Flasher"

# 2. Git Repository initialisieren
git init

# 3. Alle Dateien zur Staging Area hinzufügen
git add .

# 4. Initial Commit erstellen
git commit -m "Initial commit: IWR6843AOP ESPHome Flash Integration

- ESPHome configuration with Vital Button for OTA flash
- Complete TI mmWave bootloader protocol implementation
- Person tracking (1-5 persons) with fall detection
- Real-time progress monitoring
- Comprehensive documentation (Quick Start + Full Guide)
- Python firmware download tool
- Custom ESPHome component for advanced features
- GitHub Actions CI/CD pipeline
"

# 5. Remote Repository hinzufügen
git remote add origin https://github.com/bytelink-ai/iwr6843-esphome-flash.git

# 6. Branch auf main umbenennen (falls nötig)
git branch -M main

# 7. Push zum GitHub Repository
git push -u origin main
```

## 📝 Schritt-für-Schritt (Detailliert)

### Schritt 1: Zum Projekt-Ordner navigieren

```powershell
cd "C:\Users\Eren\Downloads\IWR6843AOP Flasher"
```

### Schritt 2: Git Repository initialisieren

```bash
git init
```

**Erwartete Ausgabe:**
```
Initialized empty Git repository in C:/Users/Eren/Downloads/IWR6843AOP Flasher/.git/
```

### Schritt 3: Git Config (falls noch nicht gesetzt)

```bash
# Dein Name
git config user.name "Dein Name"

# Deine Email
git config user.email "deine.email@example.com"

# Optional: Global setzen für alle Projekte
git config --global user.name "Dein Name"
git config --global user.email "deine.email@example.com"
```

### Schritt 4: Dateien prüfen

```bash
# Zeige alle Dateien die committed werden
git status
```

**Erwartete Ausgabe:**
```
On branch master

No commits yet

Untracked files:
  .github/
  .gitignore
  CONTRIBUTING.md
  FLASH_README.md
  GIT_SETUP.md
  LICENSE
  PROJECT_SUMMARY.md
  QUICKSTART.md
  README.md
  components/
  download_firmware.py
  flash_iwr6843aop.py
  flash_iwr6843aop_standalone.py
  iwr6843_with_flash.yaml
  requirements.txt
  secrets.yaml.example
  user_files/
  ...
```

### Schritt 5: Dateien zum Commit hinzufügen

```bash
# ALLE Dateien hinzufügen
git add .

# ODER spezifische Dateien:
git add README.md
git add iwr6843_with_flash.yaml
git add QUICKSTART.md
# ... etc
```

### Schritt 6: Status erneut prüfen

```bash
git status
```

**Erwartete Ausgabe:**
```
On branch master

No commits yet

Changes to be committed:
  new file:   .github/workflows/esphome-build.yml
  new file:   .gitignore
  new file:   CONTRIBUTING.md
  ...
```

### Schritt 7: Commit erstellen

```bash
git commit -m "Initial commit: IWR6843AOP ESPHome Flash Integration"
```

**Erwartete Ausgabe:**
```
[master (root-commit) abc1234] Initial commit: IWR6843AOP ESPHome Flash Integration
 XX files changed, XXXX insertions(+)
 create mode 100644 .github/workflows/esphome-build.yml
 create mode 100644 .gitignore
 ...
```

### Schritt 8: Remote Repository verbinden

```bash
git remote add origin https://github.com/bytelink-ai/iwr6843-esphome-flash.git
```

### Schritt 9: Remote prüfen

```bash
git remote -v
```

**Erwartete Ausgabe:**
```
origin  https://github.com/bytelink-ai/iwr6843-esphome-flash.git (fetch)
origin  https://github.com/bytelink-ai/iwr6843-esphome-flash.git (push)
```

### Schritt 10: Branch umbenennen (main statt master)

```bash
git branch -M main
```

### Schritt 11: Push zum GitHub

```bash
git push -u origin main
```

**Während des Push wirst du nach GitHub Credentials gefragt:**

**Option A: Personal Access Token (Empfohlen)**
```
Username: dein_github_username
Password: ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

**Option B: GitHub CLI**
```bash
# Installiere GitHub CLI: https://cli.github.com/
gh auth login
git push -u origin main
```

**Erwartete Ausgabe (Erfolg):**
```
Enumerating objects: XX, done.
Counting objects: 100% (XX/XX), done.
Delta compression using up to X threads
Compressing objects: 100% (XX/XX), done.
Writing objects: 100% (XX/XX), XX.XX MiB | XX.XX MiB/s, done.
Total XX (delta X), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (X/X), done.
To https://github.com/bytelink-ai/iwr6843-esphome-flash.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```

## 🎉 Fertig!

Dein Repository ist jetzt online: **https://github.com/bytelink-ai/iwr6843-esphome-flash**

## 🔄 Zukünftige Updates pushen

```bash
# 1. Änderungen machen
# ... edit files ...

# 2. Status prüfen
git status

# 3. Geänderte Dateien hinzufügen
git add .

# 4. Commit mit Nachricht
git commit -m "feat: Beschreibung der Änderung"

# 5. Push
git push
```

## 🔑 GitHub Personal Access Token erstellen

Falls du noch keinen hast:

1. Gehe zu: https://github.com/settings/tokens
2. "Generate new token" → "Classic"
3. Wähle Scopes:
   - ✅ `repo` (Full control of private repositories)
   - ✅ `workflow` (Update GitHub Action workflows)
4. "Generate token"
5. **Kopiere den Token** (wird nur einmal angezeigt!)
6. Verwende ihn als Passwort beim Push

## 🛠️ Troubleshooting

### Problem: "remote origin already exists"

```bash
# Remote entfernen
git remote remove origin

# Neu hinzufügen
git remote add origin https://github.com/bytelink-ai/iwr6843-esphome-flash.git
```

### Problem: "Permission denied"

```bash
# Prüfe Remote URL
git remote -v

# Sollte HTTPS sein, nicht SSH (außer du hast SSH Key)
# Falls SSH: Ändere zu HTTPS:
git remote set-url origin https://github.com/bytelink-ai/iwr6843-esphome-flash.git
```

### Problem: "Repository not found"

```bash
# Prüfe URL
git remote -v

# Prüfe ob du eingeloggt bist:
git credential-osxkeychain erase
host=github.com
protocol=https

# Dann erneut pushen
git push -u origin main
```

### Problem: Large File Warning

Falls Firmware-Dateien zu groß sind:

```bash
# Git LFS installieren
git lfs install

# Große Dateien tracken
git lfs track "*.bin"

# .gitattributes committen
git add .gitattributes
git commit -m "chore: Add Git LFS for binary files"
git push
```

## 📊 Nach dem Push

### Auf GitHub prüfen

1. Öffne: https://github.com/bytelink-ai/iwr6843-esphome-flash
2. Du solltest sehen:
   - ✅ README.md wird angezeigt
   - ✅ Alle Dateien sind da
   - ✅ GitHub Actions läuft (optional)

### Repository Settings

1. **About** (rechts oben): Beschreibung hinzufügen
   ```
   Over-The-Air Firmware Flasher für Texas Instruments IWR6843AOP mmWave Radar via ESPHome
   ```

2. **Topics** hinzufügen:
   - `esphome`
   - `iwr6843aop`
   - `mmwave-radar`
   - `texas-instruments`
   - `esp32`
   - `home-assistant`
   - `firmware-flash`
   - `ota-update`

3. **Website**: Deine Projekt-URL (falls vorhanden)

### GitHub Pages aktivieren (optional)

1. Settings → Pages
2. Source: "Deploy from a branch"
3. Branch: `main` → `/docs` (oder root)
4. Save

## 🌟 Nächste Schritte

1. **Star** dein eigenes Repository ⭐
2. **Share** mit der Community
3. **Create** erste Release (v1.0.0)
4. **Document** in Wiki (optional)

---

**Du hast es geschafft! 🎉**

Dein IWR6843 ESPHome Flash Projekt ist jetzt öffentlich auf GitHub!

