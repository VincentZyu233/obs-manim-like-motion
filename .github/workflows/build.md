# 构建与发布工作流说明

> **[📖 简体中文](build.md)** | **[📖 English](build.en-us.md)**

---

## 📋 概述

本项目（`obs-manim-like-motion`）的 CI/CD 流水线采用**提交信息方括号标签驱动（Bracketed Commit Tags Gating）**与**语义化版本 Tag 驱动**相结合的机制。

为了最大化节约 GitHub Actions 的计算配额与等待时间，避免每次修改 README、文档或日常零碎提交时触发长达数分钟的多平台 MSVC / GCC 深度编译，**常规提交默认在 5~9 秒内完成门禁检查并直接跳过编译**。仅在检测到特定关键字或发布标签时，才会激活全平台跨架构构建。

---

## 🔑 关键词与行为矩阵

| 触发方式 / Commit 消息中的关键词 | 检查耗时 | Windows x64 编译 | Ubuntu Linux x64 编译 | 生成 Artifacts | 自动创建 GitHub Release |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **常规提交** (不带任何关键词) | ~5-9 秒 | ⏭️ 跳过 | ⏭️ 跳过 | ❌ | ❌ |
| `[build]` / `[build-action]` / `[build-artifact]` | ~5 秒 | ✅ | ✅ | ✅ (Zip/Deb/DLL) | ❌ |
| `[release]` / `[build-release]` | ~5 秒 | ✅ | ✅ | ✅ | ✅ (自动挂载发布包) |
| **推送 Git Tag** (如 `v0.3.1`) | ~5 秒 | ✅ | ✅ | ✅ | ✅ (以 Tag 为版本发布) |
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
git commit -m "docs: update v0.3.1 notes"
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
git commit -m "release: v0.3.1 release packages [release]"
git push origin main
```

#### 方式 B：通过推送 Git Tag 触发（推荐）
```bash
git tag v0.3.1
git push origin v0.3.1
```

#### 方式 C：GitHub 网页端手动触发
1. 打开仓库的 **Actions** 标签页。
2. 在左侧选择 **Build Manim Motion Plugin** 工作流。
3. 点击右侧 **Run workflow** 下拉按钮并确认，即可无视 commit 消息直接手动启动构建。

---

## 📦 产物规格与结构说明

流水线在完成 Windows 与 Linux 构建后，会自动打包并发布多套适用不同场景的归档。各安装包内部目录结构如下：

### 1. `obs-manim-motion-<version>-windows-x64.zip`（OBS 根目录标准覆盖包）
* **压缩包内文件树**：
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
* **适用场景**：经典 OBS 安装目录覆盖安装，直接解压到 OBS 根目录（通常为 `C:\Program Files\obs-studio\`）。

### 2. `obs-manim-motion-<version>-windows-x64-OBS33.zip`（OBS 33+ 现代化独立插件架构包）
* **压缩包内文件树**：
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
* **适用场景**：OBS Studio 33+ 推荐的独立插件隔离目录，解压并入 OBS 根目录下的 `plugins/` 目录。

### 3. `obs-manim-motion-<version>-windows-x64-programdata.zip`（免管理员权限 / AppData 便携包）
* **压缩包内文件树**：
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
* **适用场景**：免管理员权限/Steam 版 OBS，将内部 `obs-manim-motion/` 解压至 `%APPDATA%\obs-studio\plugins\`。

### 4. `obs-manim-motion-<version>-x86_64-linux-gnu.deb`（Debian / Ubuntu 安装包）
* **安装路径映射**：
  * 核心库：`/usr/lib/x86_64-linux-gnu/obs-plugins/obs-manim-motion.so`
  * 资源文件：`/usr/share/obs/obs-plugins/obs-manim-motion/`
* **适用场景**：Ubuntu、Debian、Linux Mint、Deepin 等发行版，终端执行 `sudo dpkg -i <filename>.deb` 一键安装。

### 5. `obs-manim-motion-<version>-linux-x86_64.tar.gz` / `.zip`（通用 Linux 绿色便携包）
* **压缩包内文件树**：
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
* **适用场景**：Arch Linux、Fedora、openSUSE、SteamOS 等通用 Linux 环境，可解压至系统目录或用户配置目录。

---

## 🛠️ 本地安装与部署指南

无论是从 GitHub Actions 的 Artifacts 下载，还是从 GitHub Releases 下载，解压后的目录均与 OBS Studio 安装根目录保持结构对齐：

### Windows 系统部署
根据下载的压缩包类型，选择对应路径解压部署：
* **经典方式**：将 `obs-manim-motion-*-windows-x64.zip` 解压至 OBS 安装根目录（如 `C:\Program Files\obs-studio\`）。
* **免权限方式**：将 `obs-manim-motion-*-programdata.zip` 解压至 `%APPDATA%\obs-studio\plugins\`。

### Linux 系统部署
根据 Linux 发行版类型选择部署方式：
* **Debian / Ubuntu**：直接执行 `sudo dpkg -i obs-manim-motion-*.deb`。
* **通用 Linux 便携安装**：解压将 `.so` 与 `data` 放置于 `~/.config/obs-studio/plugins/obs-manim-motion/`。

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