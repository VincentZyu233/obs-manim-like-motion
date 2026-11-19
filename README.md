# obs-manim-motion

> **将 Manim 数学动画哲学与动力学曲线引入 OBS Studio 的高质量转场与动效插件。**

**中文说明** | [English Documentation (README.en-us.md)](README.en-us.md)

本项目受数学动画引擎 **[Manim (Community Edition)](https://github.com/ManimCommunity/manim)** 启发，将 Manim 标志性的缓动速度曲线（Rate Functions）、几何勾勒与两段式构建动效带入 OBS 场景转场与画面呈现中。

---

## 🌟 核心特性 (v0.1.0)

- **$M \times N$ 笛卡尔积解耦转场架构**：
  - 将转场时间轴解耦为**出场阶段（Exit）**、**微顿歇（Pause）**与**入场阶段（Entrance）**。
  - 用户可在 OBS 属性面板中自由搭配任意出场与入场动效，形成灵活的组合矩阵。
- **退出动效清单（Exit，共 2 种）**：
  - `FadeOut`（动力学加速淡出）：基于 Manim 的 `rush_into` 曲线，配合微缩下沉动力学，干净利落。
  - `Uncreate`（线框反向解构）：画面内部先向外消解，边缘留存高光线框并逆时针/向中心收拢消失。
- **入场动效清单（Entrance，共 2 种）**：
  - `FadeIn`（动力学阻尼冲入）：基于 Manim 的 `rush_from` 曲线，带有自然的阻尼刹车回弹质感。
  - `Create`（`DrawBorderThenFill` 勾勒后填充）：高亮霓虹光束沿矩形周长顺时针精准画框，随后画面纹理如墨水般展开注入。
- **高度自由的视觉与节奏调节**：
  - 支持调节出场/入场占比（Timing Split）、顿歇停顿微空隙（Pause Gap）。
  - 支持自定义边框颜色（默认经典 Manim 黄 `#FFFF00`）、线条粗细以及发光强度倍率（Glow Multiplier）。
- **零本地环境负担**：
  - 核心逻辑基于 C++20 与 `libobs` 架构，渲染层全由 GPU 像素着色器（HLSL `.effect`）实时处理。
  - 依托 GitHub Actions 自动化 CI，覆盖 **Windows x64** 与 **Ubuntu Linux x64** 的云端编译与打包。

---

## 🚀 下载与安装

### 方式一：下载预编译包（推荐）
1. 进入本仓库的 **[Actions](https://github.com/VincentZyu233/obs-manim-lile-motion/actions)** 或 Releases 页面。
2. 下载最新的 `obs-manim-motion-*-windows-x64` 构建压缩包。
3. 解压并将其放置在 OBS Studio 安装目录下：
   - 动态链接库：`<OBS安装目录>/obs-plugins/64bit/obs-manim-motion.dll`
   - 资源数据：`<OBS安装目录>/data/obs-plugins/obs-manim-motion/`
4. 启动 OBS Studio，在**转场动画（Scene Transitions）**中点击 `+` 号添加转场，选择 **Manim 动力学转场 (Manim Motion)** 即可！

---

## 🗺️ 项目规划与设计文档

* 技术调研与详细架构设计草案请参阅：[docs/plan/20261120.init/20261120.init.md](docs/plan/20261120.init/20261120.init.md)

---

## 📜 开源协议

GPL-2.0-or-later。
