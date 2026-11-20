# obs-manim-like-motion

> **Manim-inspired kinetic transitions and animation effects for OBS Studio.**

> **[📖 English](README.en-us.md)**
> **[📖 简体中文(大陆)](README.md)**

Bringing the mathematical elegance, dynamic rate functions, and geometric construction animations from **[Manim](https://github.com/ManimCommunity/manim)** directly into **OBS Studio**.

---

## 🌟 Highlights

- **$M \times N$ Cartesian Product Transition Architecture**:
  - Independent **Exit**, **Pause**, and **Entrance** phases decoupled on the normalized time pipeline.
  - Mix and match any exit and entrance animation inside OBS transition properties.
- **Exit Animations**:
  - `FadeOut`: Kinetic acceleration exit based on Manim's `rush_into` with subtle scale-down dynamics.
  - `Uncreate`: Geometric deconstruction—scene colors dissolve outward, revealing glowing wireframe contours that rewind/dissolve.
  - `Unwrite` (Rewind Handwriting Erasure · v0.3.0): Erases previous scene backwards along spatio-temporal flow field with glowing stroke tips.
- **Entrance Animations**:
  - `FadeIn`: Kinetic deceleration landing based on Manim's `rush_from` with smooth settling dynamics.
  - `Create` (`DrawBorderThenFill`): High-intensity outline drawn on canvas perimeter and internal scene elements, followed by a fluid color fill wave.
  - `Write` (Spatio-Temporal Handwriting · v0.3.0): Replicates classic Manim mathematical derivation handwriting. Supports Natural LTR, Boustrophedon snake scan, Vertical code waterfall, Diagonal sweep, and Polar logarithmic spiral (`SpiralIn`), backed by `lag_ratio` infill delays and pen tip sparkles.
- **Real-Time GPU Contour Extraction**:
  - Utilizes 3x3 Scharr gradient operators and Fourier harmonic wave modulation to transform any real-time screen into high-contrast chalkboard sketches in sub-millisecond shader passes.
  - 3-way mode switch: **`GPU Real-Time Sketch (XDoG)`**, **`Fourier Harmonic Waves`**, and **`Disabled (Border Only)`**.
- **Stroke Color Preset Palette**:
  - 7 preset themes (Mizuiro mist blue `#7FA8E8`, Ice Blue `#DCEBFF`, Chalk White `#FFFFFF`, Math Gold `#FFD700`, Ribbon Pink `#F4C6D7`, Cream White `#FFF3DE`, Cyber Neon Cyan `#00FFFF`) plus custom RGBA color picker.
  - Instant synchronization between dropdown presets and native OBS color swatch.
- **Fine-Grained Dynamic Controls**:
  - Contour Sensitivity & Fill-In Delay sliders.
  - Exit timing split ratio & pause gap duration.
  - Border stroke thickness & glow multiplier.
- **Zero Local Overhead**:
  - Pure GPU rendering via HLSL shaders (`.effect`) in modern C++20.
  - Automated cloud builds for **Windows x64** and **Linux x64** via GitHub Actions CI.

---

## 🛠️ Automated CI/CD & Build Workflow

Cross-platform builds and releases are automated via GitHub Actions, featuring a `winload`-style commit keyword gating mechanism to prevent daily commits from consuming compute resources. For detailed trigger guides, please refer to:
* 📖 [Build & Release Workflow (.github/workflows/build.en-us.md)](.github/workflows/build.en-us.md)

---

## 🛠️ Download & Installation

### Official Releases (GitHub Releases)
Head over to the **[Releases](https://github.com/VincentZyu233/obs-manim-like-motion/releases)** page to download the latest prebuilt packages:

#### 🪟 Windows x64
Download `obs-manim-motion-*-windows-x64.zip` and extract into your OBS installation directory:
* DLL: `<OBS_INSTALL_DIR>/obs-plugins/64bit/obs-manim-motion.dll`
* Data & Shaders: `<OBS_INSTALL_DIR>/data/obs-plugins/obs-manim-motion/`

#### 🐧 Linux x64 (Ubuntu / Debian / Generic Linux)
* **Debian / Ubuntu**: Download `.deb` package and run `sudo dpkg -i obs-manim-motion-*.deb`.
* **Generic Linux**: Extract into your user plugin folder:
  * Shared Library: `~/.config/obs-studio/plugins/obs-manim-motion/bin/64bit/obs-manim-motion.so`
  * Data: `~/.config/obs-studio/plugins/obs-manim-motion/data/`

After installation, launch OBS Studio, click the **`+`** button in the **Scene Transitions** dock, and choose **Manim Motion**!

---

## 📜 License

GPL-2.0-or-later.
