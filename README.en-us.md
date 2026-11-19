# obs-manim-like-motion

> **Manim-inspired kinetic transitions and animation effects for OBS Studio.**

> **[📖 English](README.en-us.md)**
> **[📖 简体中文(大陆)](README.md)**

Bringing the mathematical elegance, dynamic rate functions, and geometric construction animations from **[Manim](https://github.com/ManimCommunity/manim)** directly into **OBS Studio**.

---

## 🌟 Highlights (v0.1.0)

- **$M \times N$ Cartesian Product Transition Architecture**:
  - Independent **Exit** and **Entrance** phases decoupled on the normalized time pipeline.
- **Exit Animations ($M=2$)**:
  - `FadeOut`: Kinetic acceleration exit based on Manim's `rush_into` with subtle scale-down dynamics.
  - `Uncreate`: Geometric border rewind—interior dissolves outward, revealing a glowing border that unwinds counter-clockwise.
- **Entrance Animations ($N=2$)**:
  - `FadeIn`: Kinetic deceleration landing based on Manim's `rush_from` with smooth settling dynamics.
  - `Create` (`DrawBorderThenFill`): High-intensity neon outline drawn clockwise along the canvas perimeter, followed by a radial fill wave unveiling the incoming scene.
- **Customizable Timing & Aesthetics**:
  - Adjust Exit/Entrance ratio, pause gap, border color (default Manim Yellow), border thickness, and glow intensity.
- **Zero Local Overhead**:
  - Pure GPU rendering via HLSL shaders (`.effect`).
  - Automated cloud builds for **Windows x64** and **Linux x64** via GitHub Actions CI.

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
