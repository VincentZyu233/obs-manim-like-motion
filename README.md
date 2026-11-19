# obs-manim-motion

> **Manim-inspired kinetic transitions and animation effects for OBS Studio.**

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

## 🛠️ Building & Installation

### Download Prebuilt Binaries (Recommended)
1. Head to the **[Actions](https://github.com/VincentZyu233/obs-manim-lile-motion/actions)** tab or Releases.
2. Download the latest `obs-manim-motion-*-windows-x64` zip archive.
3. Extract `obs-manim-motion.dll` and the `data/obs-plugins/obs-manim-motion` folder into your OBS Studio directory:
   - DLL: `<OBS_INSTALL_DIR>/obs-plugins/64bit/obs-manim-motion.dll`
   - Data: `<OBS_INSTALL_DIR>/data/obs-plugins/obs-manim-motion/`
4. Launch OBS Studio, add a new Transition under **Scene Transitions**, and choose **Manim Motion**!

---

## 📜 License
GPL-2.0-or-later.
