# obs-manim-like-motion

> **将 Manim 数学动画哲学与动力学曲线引入 OBS Studio 的高质量转场与动效插件。**

> **[📖 English](README.en-us.md)**
> **[📖 简体中文(大陆)](README.md)**

本项目受数学动画引擎 **[Manim (Community Edition)](https://github.com/ManimCommunity/manim)** 启发，将 Manim 标志性的缓动速度曲线（Rate Functions）、几何勾勒与两段式构建动效带入 OBS 场景转场与画面呈现中。

---

## 🌟 核心特性

- **$M \times N$ 笛卡尔积解耦转场架构**：
  - 将转场时间轴解耦为**出场阶段（Exit）**、**微顿歇（Pause）**与**入场阶段（Entrance）**。
  - 用户可在单个转场属性面板中自由搭配任意出场与入场动效，形成灵活组合。
- **退出动效清单（Exit）**：
  - `FadeOut`（动力学加速淡出）：基于 Manim 的 `rush_into` 曲线，配合微缩下沉动力学，干净利落。
  - `Uncreate`（线框反向解构）：画面内部先向外消解，边缘与元素留存高光线稿骨架，随后逆时针/向中心收拢消散。
  - `Unwrite`（倒带式手绘擦除 · v0.3.0）：沿时空流场反向抹除画面，伴随笔尖高光微斑与线稿时差褪色。
- **入场动效清单（Entrance）**：
  - `FadeIn`（动力学阻尼冲入）：基于 Manim 的 `rush_from` 曲线，带有自然的阻尼刹车回弹质感。
  - `Create`（`DrawBorderThenFill` 勾勒后填充）：暗场中先由高光线稿勾勒出所有文字、边框与主体轮廓，随后画面真实色彩如墨水般由内向外注入浸润。
  - `Write`（时空流场手写演算 · v0.3.0）：复刻 Manim 经典的文字手绘推导过程，支持自然逐行横排、折返蛇形、代码瀑布、对角光幕与极坐标螺旋（`SpiralIn`）5 种流向，以及细致的 `lag_ratio` 时差滞后与笔尖火花。
- **GPU 实时几何轮廓素描场（Contour Extraction）**：
  - 采用 3×3 Scharr 梯度卷积算子与频域谐波算法，无需矢量字体或离线预处理，单 Shader Pass 直接在毫秒内将实时画面转换为数学黑板手绘线稿。
  - 提供 **`GPU 实时线稿素描 (XDoG 算子)`**、**`频域傅里叶谐波`** 与 **`关闭 (仅保留外边框)`** 3 种模式下拉切换。
- **线稿高光画风预设矩阵（Color Preset）**：
  - 提供 7 种经典调色预设（水色雾蓝 `#7FA8E8`、冰蓝微光 `#DCEBFF`、经典粉笔白 `#FFFFFF`、黄金流光 `#FFD700`、缎带柔粉 `#F4C6D7`、奶油温白 `#FFF3DE`、赛博霓虹青 `#00FFFF`）以及自定义 RGBA 拾色器。
  - 下拉选单与原生调色板即时联动同步。
- **精细化动力学与节奏调节**：
  - 轮廓提取灵敏度（Contour Sensitivity）、色彩浸润注入延迟（Fill-In Delay）。
  - 出场/入场占比（Timing Split）、顿歇停顿微空隙（Pause Duration）。
  - 边框线条粗细（Border Thickness）、发光强度倍率（Glow Multiplier）。
- **零本地环境负担**：
  - 核心逻辑基于现代 C++20 与 `libobs` 架构，渲染层全由 GPU 像素着色器（HLSL `.effect`）实时处理，开销极低。
  - 依托 GitHub Actions 自动化 CI，覆盖 **Windows x64** 与 **Ubuntu Linux x64** 的云端编译与打包。

---

## 🛠️ 自动化构建与 CI/CD

本项目通过 GitHub Actions 实现了全自动跨平台构建与发布流水线，采用与 `winload` 一致的提交关键字门禁机制，避免日常提交消耗计算资源。详细触发指南请参阅：
* 📖 [构建与发布工作流说明 (.github/workflows/build.md)](.github/workflows/build.md)

---

## 🚀 下载与安装

### 获取正式发布版（GitHub Releases）
请前往项目的 **[Releases](https://github.com/VincentZyu233/obs-manim-like-motion/releases)** 页面下载对应系统的最新发布包：

#### 🪟 Windows x64
下载 `obs-manim-motion-*-windows-x64.zip`，解压至你的 OBS 安装目录：
* 动态链接库：`<OBS安装目录>/obs-plugins/64bit/obs-manim-motion.dll`
* 资源与着色器：`<OBS安装目录>/data/obs-plugins/obs-manim-motion/`

#### 🐧 Linux x64 (Ubuntu / Debian / Arch / Fedora 等通用发行版)
* **Debian / Ubuntu / Mint**：下载 `.deb` 包直接执行 `sudo dpkg -i obs-manim-motion-*.deb` 即可。
* **通用 Linux / Steam Deck (便携安装)**：下载 `obs-manim-motion-*-linux-x86_64.tar.gz` 或 `.zip`：
  * 系统级：解压到 `/usr/local/`（包含 `lib/obs-plugins/` 与 `share/obs/obs-plugins/`）；
  * 用户级：解压将动态库与数据放置在 `~/.config/obs-studio/plugins/obs-manim-motion/`。

安装完成后启动 OBS Studio，在右下角 **转场动画** 停靠窗口点击 **`+`** 号选择 **Manim 动力学转场 (Manim Motion)** 即可配置使用！

> [!TIP]
> **插件热更新提示**：若在 OBS Studio 正在运行期间替换或更新了插件文件（`.dll`、`.so` 或 `.effect` 资源），**必须重启 OBS Studio** 才能加载最新编译的二进制文件、属性面板配置项及着色器特效。

---

## 📜 开源协议

GPL-2.0-or-later。
