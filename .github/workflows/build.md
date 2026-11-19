# 构建与发布工作流说明

> **[📖 简体中文](build.md)** | **[📖 English](build.en-us.md)**

---

## 📋 概述

本项目（`obs-manim-like-motion`）的 CI/CD 流水线采用**提交信息方括号标签驱动（Bracketed Commit Tags Gating）**与**语义化版本 Tag 驱动**相结合的机制。

为了最大化节约 GitHub Actions 的计算配额与等待时间，避免每次修改 README、文档或日常零碎提交时触发长达 10~15 分钟的多平台 MSVC / GCC 深度编译，**常规提交默认在 5~9 秒内完成门禁检查并直接跳过编译**。仅在检测到特定关键字或发布标签时，才会激活全平台跨架构构建。

---

## 🔑 关键词与行为矩阵

| 触发方式 / Commit 消息中的关键词 | 检查耗时 | Windows x64 编译 | Ubuntu Linux x64 编译 | 生成 Artifacts | 自动创建 GitHub Release |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **常规提交** (不带任何关键词) | ~5-9 秒 | ⏭️ 跳过 | ⏭️ 跳过 | ❌ | ❌ |
| `[build]` / `[build-action]` / `[build-artifact]` | ~5 秒 | ✅ | ✅ | ✅ (Zip/Deb/DLL) | ❌ |
| `[release]` / `[build-release]` | ~5 秒 | ✅ | ✅ | ✅ | ✅ (自动挂载发布包) |
| **推送 Git Tag** (如 `v0.1.0`) | ~5 秒 | ✅ | ✅ | ✅ | ✅ (以 Tag 为版本发布) |
| **手动网页触发** (`workflow_dispatch`) | ~5 秒 | ✅ | ✅ | ✅ | ❌ |

> [!NOTE]
> **触发格式规范：**
> 1. 只有完整的英文方括号标签（如 `[build]` 或 `[build-release]`）才会被匹配触发。
> 2. 标签支持大小写不敏感匹配（如 `[BUILD]`、`[build]` 均可）。
> 3. 不带方括号的裸词（如 `build something`）或圆括号（如 `(build)`）**不会**触发构建，防止日常提交被误伤。

---

## 🚀 用法示例

### 1. 日常开发与文档修改（默认跳过构建）
修改文档、注释或代码草稿时，直接正常提交，CI 会在 10 秒内绿灯通过，不消耗任何 runner 分钟数：
```bash
git commit -m "docs: update v0.2.0 contour research report"
git push origin main
```

### 2. 按需触发全平台构建（验证编译与下载 Artifacts）
当完成了着色器代码或 C++ 核心逻辑编写，需要远程 Windows 和 Linux 虚拟机验证编译正确性，并下载打包产物在本地 OBS 测试时：
```bash
git commit -m "feat(shader): implement xdog edge extraction filter [build]"
git push origin main
```
也可以在空提交中单独触发验证：
```bash
git commit --allow-empty -m "ci: verify cross-platform compilation [build-action]"
git push origin main
```

### 3. 正式发布版本（构建 + 创建 GitHub Releases）

#### 方式 A：通过 Commit 消息触发
```bash
git commit -m "release: v0.2.0 add watercolor stage theme and contour sketches [release]"
git push origin main
```

#### 方式 B：通过推送 Git Tag 触发（推荐）
```bash
git tag v0.2.0
git push origin v0.2.0
```

#### 方式 C：GitHub 网页端手动触发
1. 打开仓库的 **Actions** 标签页。
2. 在左侧选择 **Build Manim Motion Plugin** 工作流。
3. 点击右侧 **Run workflow** 下拉按钮并确认，即可无视 commit 消息直接手动启动构建。

---

## 📦 产物规格与结构说明

构建流水线在编译成功后，会自动打包跨平台产物：

### 1. Windows x64 产物包
* **归档名称**：`obs-manim-motion-<version>-windows-x64-<hash>.zip`
* **包含内容**：
  * `obs-plugins/64bit/obs-manim-motion.dll`：插件核心动态链接库。
  * `obs-plugins/64bit/obs-manim-motion.pdb`：MSVC 调试符号文件（排查崩溃用）。
  * `data/obs-plugins/obs-manim-motion/`：包含 `manim_transition.effect` 着色器文件与中英文语言包。

### 2. Ubuntu Linux x86_64 产物包
* **归档名称**：`obs-manim-motion-<version>-ubuntu-x86_64-<hash>.tar.gz` / `.deb`
* **包含内容**：
  * `obs-plugins/obs-manim-motion.so`：Linux 动态共享库。
  * `data/obs-plugins/obs-manim-motion/`：资源与着色器。

---

## 🛠️ 本地安装与部署指南

无论是从 GitHub Actions 的 Artifacts 下载，还是从 GitHub Releases 下载，解压后的目录均与 OBS Studio 安装根目录保持结构对齐：

### Windows 系统
解压后将对应文件放入 OBS 安装目录（例如 `D:\SSoftwareFiles\obs\OBS-Studio-32.1.2-Windows-x64\`）：
```
<OBS安装根目录>/
├── obs-plugins/
│   └── 64bit/
│       ├── obs-manim-motion.dll
│       └── obs-manim-motion.pdb (可选)
└── data/
    └── obs-plugins/
        └── obs-manim-motion/
            ├── manim_transition.effect
            └── locale/
                ├── zh-CN.ini
                └── en-US.ini
```

### Linux 系统
可直接安装 `.deb` 包，或者将 `.so` 与 `data` 放置于：
* 用户级目录：`~/.config/obs-studio/plugins/obs-manim-motion/`
* 系统级目录：`/usr/lib/obs-plugins/` 与 `/usr/share/obs/obs-plugins/`

---

## 🏗️ 工作流内部执行流程

```mermaid
flowchart TD
    A["Push Event / Tag / Dispatch"] --> B["check (检查触发门禁)"]
    B -->|常规 Commit 无标签| C["⏭️ 跳过构建 (5秒完成，省配额)"]
    B -->|匹配 [build] / [release] / Tag| D["全平台并行构建"]
    
    subgraph 并行编译矩阵
        D --> E["windows-build (MSVC C++20 + InnoSetup)"]
        D --> F["ubuntu-build (GCC-13 + CPack)"]
    end
    
    E --> G["上传 Windows Artifacts (.zip / .dll)"]
    F --> H["上传 Ubuntu Artifacts (.deb / .so)"]
    
    G --> I{"是否触发 Release?\n([release] 或 Tag)"}
    H --> I
    
    I -->|是| J["release Job (gh release create 发布安装包)"]
    I -->|否| K["🎉 构建完成 (供开发测试下载)"]
```
