# Build & Release Workflow

> **[📖 简体中文](build.md)** | **[📖 English](build.en-us.md)**

---

## 📋 Overview

The CI/CD pipeline for `obs-manim-like-motion` is driven by **bracketed commit tags** and **semantic version Git tags**.

To conserve GitHub Actions runner minutes and eliminate unnecessary build wait times on documentation edits or small commits, **regular commits without trigger tags finish within 5 to 9 seconds by skipping heavy compilation jobs**. Only commits containing recognized keywords or version tags trigger cross-platform builds.

---

## 🔑 Keywords & Trigger Matrix

| Trigger / Commit Keyword | Check Duration | Windows x64 Build | Ubuntu Linux x64 Build | Generate Artifacts | Create GitHub Release |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Regular Commit** (no tags) | ~5-9s | ⏭️ Skipped | ⏭️ Skipped | ❌ | ❌ |
| `[build]` / `[build-action]` / `[build-artifact]` | ~5s | ✅ | ✅ | ✅ (Zip/Deb/DLL) | ❌ |
| `[release]` / `[build-release]` | ~5s | ✅ | ✅ | ✅ | ✅ (Publish release assets) |
| **Push Git Tag** (e.g. `v0.3.1`) | ~5s | ✅ | ✅ | ✅ | ✅ (Tagged Release) |
| **Manual Web Dispatch** (`workflow_dispatch`) | ~5s | ✅ | ✅ | ✅ | ❌ |

> [!NOTE]
> **Trigger Format Rules:**
> 1. Only full bracketed tags like `[build]` or `[build-release]` trigger the build.
> 2. Matching is case-insensitive (e.g. `[BUILD]` and `[build]` are treated identically).
> 3. Bare words (e.g. `build something`) or parentheses (e.g. `(build)`) will **not** trigger builds, preventing unintended execution.

---

## 🚀 Usage Examples

### 1. Regular Development & Docs (Skip Build by Default)
For commits modifying documentation, comments, or draft notes, commit normally. CI verifies conditions and passes in ~10 seconds:
```bash
git commit -m "docs: update v0.3.1 notes"
git push origin main
```

### 2. On-Demand Full Multi-Platform Build (Test & Download Artifacts)
When shader code or C++ logic changes need verification across Windows and Linux, and you want to download prebuilt artifacts for local testing:
```bash
git commit -m "feat(shader): implement xdog edge extraction filter [build]"
git push origin main
```
You can also trigger it on an empty commit:
```bash
git commit --allow-empty -m "ci: verify cross-platform compilation [build-action]"
git push origin main
```

### 3. Official Release (Build + Create GitHub Release)

#### Option A: Trigger via Commit Message
```bash
git commit -m "release: v0.3.1 release packages [release]"
git push origin main
```

#### Option B: Trigger via Git Tag (Recommended)
```bash
git tag v0.3.1
git push origin v0.3.1
```

#### Option C: Manual Trigger via GitHub Web UI
1. Navigate to the **Actions** tab in the repository.
2. Select the **Build Manim Motion Plugin** workflow on the left.
3. Click the **Run workflow** dropdown on the right and confirm.

---

## 📦 Artifacts & Package Structure

Upon completing Windows and Linux compilation, the pipeline packages multiple archives for different scenarios. Internal structures are detailed below:

### 1. `obs-manim-motion-<version>-windows-x64.zip` (Standard Root Overlay Package)
* **Internal Folder Tree**:
  ```text
  obs-manim-motion-<version>-windows-x64.zip
  ├── obs-plugins/
  │   └── 64bit/
  │       └── obs-manim-motion.dll
  └── data/
      └── obs-plugins/
          └── obs-manim-motion/
              ├── locale/
              │   ├── en-US.ini
              │   └── zh-CN.ini
              └── effects/
                  └── manim_transition.effect
  ```
* **Target Scenario**: Classic OBS installation overlay; extract directly into OBS root directory (usually `C:\Program Files\obs-studio\`).

### 2. `obs-manim-motion-<version>-windows-x64-OBS33.zip` (OBS 33+ Modern Modular Package)
* **Internal Folder Tree**:
  ```text
  obs-manim-motion-<version>-windows-x64-OBS33.zip
  └── plugins/
      └── obs-manim-motion/
          ├── bin/
          │   └── 64bit/
          │       └── obs-manim-motion.dll
          └── data/
              ├── locale/
              │   ├── en-US.ini
              │   └── zh-CN.ini
              └── effects/
                  └── manim_transition.effect
  ```
* **Target Scenario**: Modern isolated plugin directory for OBS 33+; extracts and merges into `plugins/` in the OBS root directory.

### 3. `obs-manim-motion-<version>-windows-x64-programdata.zip` (Non-Admin / AppData Portable Package)
* **Internal Folder Tree**:
  ```text
  obs-manim-motion-<version>-windows-x64-programdata.zip
  └── obs-manim-motion/
      ├── bin/
      │   └── 64bit/
      │       └── obs-manim-motion.dll
      └── data/
          ├── locale/
          │   ├── en-US.ini
          │   └── zh-CN.ini
          └── effects/
              └── manim_transition.effect
  ```
* **Target Scenario**: Non-admin environments / Steam OBS; extract inner `obs-manim-motion/` into `%APPDATA%\obs-studio\plugins\`.

### 4. `obs-manim-motion-<version>-x86_64-linux-gnu.deb` (Debian / Ubuntu Package)
* **Installed File Mappings**:
  * Shared Binary: `/usr/lib/x86_64-linux-gnu/obs-plugins/obs-manim-motion.so`
  * Resource Data: `/usr/share/obs/obs-plugins/obs-manim-motion/`
* **Target Scenario**: Ubuntu, Debian, Linux Mint, Pop!_OS, etc. Install via terminal with `sudo dpkg -i <filename>.deb`.

### 5. `obs-manim-motion-<version>-linux-x86_64.tar.gz` / `.zip` (Generic Linux Portable Tarball)
* **Internal Folder Tree**:
  ```text
  ├── lib/
  │   └── obs-plugins/
  │       └── obs-manim-motion.so
  └── share/
      └── obs/
          └── obs-plugins/
              └── obs-manim-motion/
                  ├── locale/
                  └── effects/
  ```
* **Target Scenario**: Arch Linux, Fedora, openSUSE, SteamOS, etc. Extract into system path or user config directory.

---

## 🛠️ Local Installation & Deployment

Extracted packages mirror the folder structure of the standard OBS Studio directory for straightforward manual installation:

### Windows Deployment
Choose the target destination according to the downloaded archive flavor:
* **Classic Method**: Extract `obs-manim-motion-*-windows-x64.zip` into OBS root (e.g. `C:\Program Files\obs-studio\`).
* **Non-Admin Method**: Extract `obs-manim-motion-*-programdata.zip` into `%APPDATA%\obs-studio\plugins\`.

### Linux Deployment
Choose the installation path based on your Linux distribution:
* **Debian / Ubuntu**: Run `sudo dpkg -i obs-manim-motion-*.deb` directly.
* **Generic Linux Portable**: Extract `.so` and `data` into `~/.config/obs-studio/plugins/obs-manim-motion/`.

---

## 🏗️ Workflow Pipeline Architecture

```mermaid
flowchart TD
    A["Push Event / Tag / Dispatch"] --> B["check (Inspect Trigger Conditions)"]
    B -->|Regular Commit Without Tags| C["⏭️ Skip Compilation (Done in 5s)"]
    B -->|Matches [build] / [release] / Tag| D["Parallel Multi-Platform Build"]
    
    subgraph Parallel Matrix
        D --> E["windows-build (MSVC C++20 + InnoSetup)"]
        D --> F["ubuntu-build (GCC-13 + CPack)"]
    end
    
    E --> G["Upload Windows Artifacts (.zip / .dll)"]
    F --> H["Upload Ubuntu Artifacts (.deb / .so)"]
    
    G --> I{"Should Create Release?\n([release] or Tag)"}
    H --> I
    
    I -->|Yes| J["release Job (gh release create with packages)"]
    I -->|No| K["🎉 Build Complete (Available in Artifacts)"]
```