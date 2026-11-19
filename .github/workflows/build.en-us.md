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
| **Push Git Tag** (e.g. `v0.1.0`) | ~5s | ✅ | ✅ | ✅ | ✅ (Tagged Release) |
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
git commit -m "docs: update v0.2.0 contour research report"
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
git commit -m "release: v0.2.0 add watercolor stage theme and contour sketches [release]"
git push origin main
```

#### Option B: Trigger via Git Tag (Recommended)
```bash
git tag v0.2.0
git push origin v0.2.0
```

#### Option C: Manual Trigger via GitHub Web UI
1. Navigate to the **Actions** tab in the repository.
2. Select the **Build Manim Motion Plugin** workflow on the left.
3. Click the **Run workflow** dropdown on the right and confirm.

---

## 📦 Artifacts & Package Structure

Upon a successful build, the workflow produces standardized packages:

### 1. Windows x64 Artifacts
* **Archive Name**: `obs-manim-motion-<version>-windows-x64-<hash>.zip`
* **Contents**:
  * `obs-plugins/64bit/obs-manim-motion.dll`: Core plugin binary.
  * `obs-plugins/64bit/obs-manim-motion.pdb`: Debug symbols for crash diagnostics.
  * `data/obs-plugins/obs-manim-motion/`: Includes `manim_transition.effect` shader and locale files.

### 2. Ubuntu Linux x86_64 Artifacts
* **Archive Name**: `obs-manim-motion-<version>-ubuntu-x86_64-<hash>.tar.gz` / `.deb`
* **Contents**:
  * `obs-plugins/obs-manim-motion.so`: Linux shared library.
  * `data/obs-plugins/obs-manim-motion/`: Resources and shaders.

---

## 🛠️ Local Installation & Deployment

Extracted packages mirror the folder structure of the standard OBS Studio directory:

### Windows
Copy the contents into your OBS Studio installation root directory (e.g. `D:\SSoftwareFiles\obs\OBS-Studio-32.1.2-Windows-x64\`):
```
<OBS Root Directory>/
├── obs-plugins/
│   └── 64bit/
│       ├── obs-manim-motion.dll
│       └── obs-manim-motion.pdb (optional)
└── data/
    └── obs-plugins/
        └── obs-manim-motion/
            ├── manim_transition.effect
            └── locale/
                ├── zh-CN.ini
                └── en-US.ini
```

### Linux
Install the `.deb` package directly, or place `.so` and `data` in:
* User level: `~/.config/obs-studio/plugins/obs-manim-motion/`
* System level: `/usr/lib/obs-plugins/` and `/usr/share/obs/obs-plugins/`

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
