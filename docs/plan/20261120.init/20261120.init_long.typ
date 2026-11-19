// ============================================================
// OBS Manim-Like Motion 插件调研与初始规划 (v0.1.0)
// 连续长图排版 (参考 TiYunBei 连续数学卷 height: auto 架构)
// ============================================================

#set page(
  width: 210mm,
  height: auto, // 核心：高度自适应，连续无缝长图，绝无中间切断缝隙
  margin: (x: 18mm, y: 18mm),
  fill: rgb("#ffffff"),
)

#let kai-fonts = (
  "LXGW WenKai Mono",
  "Microsoft YaHei",
)

#let code-fonts = (
  "Consolas",
  "Courier New",
)

#set text(
  font: kai-fonts,
  size: 10pt,
  fill: rgb("#1e293b"),
  lang: "zh",
)

#set par(justify: true, leading: 0.82em)

// 色彩定义
#let c-primary = rgb("#0284c7")
#let c-dark = rgb("#0f172a")
#let c-border = rgb("#e2e8f0")
#let c-card-bg = rgb("#f8fafc")
#let c-muted = rgb("#64748b")
#let c-accent = rgb("#8b5cf6")

// 标题样式
#show heading.where(level: 1): it => block(below: 12pt, above: 18pt)[
  #text(size: 20pt, weight: "bold", fill: c-primary)[#it.body]
  #v(3pt)
  #line(length: 100%, stroke: 1.5pt + c-primary)
]

#show heading.where(level: 2): it => block(below: 8pt, above: 14pt)[
  #text(size: 13pt, weight: "bold", fill: c-dark)[#it.body]
  #v(2pt)
  #line(length: 100%, stroke: 0.8pt + c-border)
]

#show heading.where(level: 3): it => block(below: 6pt, above: 10pt)[
  #text(size: 11pt, weight: "bold", fill: c-primary)[#it.body]
]

// 代码高亮
#show raw: it => box(
  fill: rgb("#f1f5f9"),
  inset: (x: 4pt, y: 2pt),
  radius: 3pt,
  text(font: code-fonts, size: 8.8pt, fill: rgb("#0284c7"))[#it.text]
)

#show raw.where(block: true): it => block(
  fill: rgb("#f8fafc"),
  stroke: 0.8pt + c-border,
  inset: 10pt,
  radius: 5pt,
  width: 100%,
  text(font: code-fonts, size: 8.6pt, fill: rgb("#0f172a"))[#it.text]
)

// 容器盒函数
#let card-box(title: none, icon: none, stroke-color: c-border, fill-color: c-card-bg, body) = block(
  width: 100%,
  fill: fill-color,
  stroke: 1pt + stroke-color,
  radius: 6pt,
  inset: 12pt,
  spacing: 8pt,
)[
  #if title != none [
    #grid(
      columns: (auto, 1fr),
      gutter: 8pt,
      align: horizon,
      if icon != none { text(size: 13pt)[#icon] } else { none },
      text(weight: "bold", size: 10.5pt, fill: if stroke-color != c-border { stroke-color } else { c-dark })[#title]
    )
    #v(4pt)
  ]
  #body
]

// 头部标题区
#align(center)[
  #v(4pt)
  #text(size: 21pt, weight: "bold", fill: c-primary)[OBS Manim-Like Motion 插件]
  #v(2pt)
  #text(size: 13pt, weight: "medium", fill: c-dark)[架构调研与初始规划指南 (v0.1.0)]
  #v(4pt)
  #text(size: 9pt, fill: c-muted)[跨端 OBS 场景转场动效 · 借鉴数学动画引擎质感 · 笛卡尔积组合渲染]
  #v(8pt)
]

#card-box(title: "项目元信息与上游依赖", icon: "📌", stroke-color: c-primary, fill-color: rgb("#f0f9ff"))[
  - *文档日期*：`2026-11-20`
  - *项目路径*：`D:/aaaStuffsaaa/from_git/github/obs/obs-manim-lile-motion`
  - *参考上游仓库*：
    - `Manim Community Edition`：数学动画渲染拓扑与动力学缓动
    - `obs-move-transition`：OBS 转场插件渲染接口与时间轴插值
    - `OBS Studio SDK`：底层图形渲染管道与 HLSL Shader 接口
]

== 1. 项目愿景与背景

在传统直播与录屏场景中，OBS 自带的转场（淡入淡出、滑入滑出、亮度擦除等）偏向线性或传统剪辑软件风格，缺乏结构化讲解和数学动效的质感。

本项目旨在*借鉴并移植 Manim（知名数学动画引擎）的标志性动画哲学与动力学算法*，为 OBS 打造一款轻量、现代且具备高级“构建感”的转场与动效插件。

== 2. 核心架构：两段式笛卡尔积解耦模型 ($M times N$)

传统 OBS 转场通常为单段式硬编码（场景 A 与场景 B 强耦合绑定）。本项目采用*解耦的两段式时间轴管道*：

#block(
  width: 100%,
  fill: rgb("#f8fafc"),
  stroke: 1pt + rgb("#e2e8f0"),
  radius: 8pt,
  inset: 12pt,
  spacing: 8pt,
)[
  #text(weight: "bold", size: 9pt, fill: rgb("#475569"))[⏱️ 时间轴归一化动力学解耦模型 $t in [0.0, 1.0]$]
  #v(6pt)
  #grid(
    columns: (1fr, auto, 0.42fr, auto, 1fr),
    align: horizon,
    gutter: 8pt,
    block(
      fill: rgb("#fff7ed"),
      stroke: 1.2pt + rgb("#f97316"),
      radius: 6pt,
      inset: (x: 8pt, y: 7pt),
      width: 100%,
    )[
      #align(center)[
        #text(weight: "bold", size: 9pt, fill: rgb("#c2410c"))[阶段 1: Exit 出场 ($t in [0, 0.40]$)]
        #v(3pt)
        #text(size: 7.5pt, fill: rgb("#475569"))[场景 A 独立退场动效\ #text(fill: rgb("#ea580c"), weight: "bold")[rush_into] 动力学先缓后急淡出 / Uncreate]
      ]
    ],
    text(size: 14pt, fill: rgb("#94a3b8"))[→],
    block(
      fill: rgb("#f1f5f9"),
      stroke: 1.2pt + rgb("#94a3b8"),
      radius: 6pt,
      inset: (x: 6pt, y: 7pt),
      width: 100%,
    )[
      #align(center)[
        #text(weight: "bold", size: 8.5pt, fill: rgb("#475569"))[阶段 2: Pause]
        #v(3pt)
        #text(size: 7pt, fill: rgb("#64748b"))[$t in [0.40, 0.45]$\ 极短停顿营造呼吸感]
      ]
    ],
    text(size: 14pt, fill: rgb("#94a3b8"))[→],
    block(
      fill: rgb("#f0fdf4"),
      stroke: 1.2pt + rgb("#16a34a"),
      radius: 6pt,
      inset: (x: 8pt, y: 7pt),
      width: 100%,
    )[
      #align(center)[
        #text(weight: "bold", size: 9pt, fill: rgb("#15803d"))[阶段 3: Entrance 入场 ($t in [0.45, 1.0]$)]
        #v(3pt)
        #text(size: 7.5pt, fill: rgb("#475569"))[场景 B 独立进场动效\ #text(fill: rgb("#16a34a"), weight: "bold")[rush_from] 动力学阻尼减速落地 / Create]
      ]
    ]
  )
  #v(6pt)
  #line(length: 100%, stroke: (paint: rgb("#cbd5e1"), dash: "dashed"))
  #v(2pt)
  #grid(
    columns: (1fr, 1fr),
    gutter: 12pt,
    text(size: 7.5pt, fill: rgb("#64748b"))[• 出场插值: $t_("exit") = "clamp"(t / 0.40, 0, 1)$],
    align(right)[#text(size: 7.5pt, fill: rgb("#64748b"))[• 进场插值: $t_("enter") = "clamp"((t - 0.45) / 0.55, 0, 1)$]],
  )
]

=== 笛卡尔积解耦优势
1. *关注点分离*：Exit 渲染器与 Entrance 渲染器完全独立，只操作各自源纹理与归一化时间变量 $t_("rel") in [0, 1]$；
2. *高组合自由度*：只需实现 $M$ 个退出效果与 $N$ 个进入效果，即可在 OBS 属性面板中提供 $M times N$ 种自由组合；
3. *极佳扩展性*：未来新增任何一种入场动效，现有所有出场动效均可自动搭配复用。

== 3. v0.1.0 最小可行性范围（MVP: $2 times 2 = 4$ 组合）

首个版本坚持克制原则，专注于 *`Create` 族系与动力学淡入淡出*，暂不引入复杂的矢量字形手写（`Write`）：

#grid(
  columns: (1fr, 1fr),
  gutter: 10pt,
  card-box(title: "退出动效清单 (Exit, M = 2)", icon: "🚪", stroke-color: rgb("#ea580c"), fill-color: rgb("#fff7ed"))[
    *1. FadeOut（动力学加速淡出）*
    - *数学核心*：Manim `rush_into` 缓动函数（先缓后急，加速退出）；
    - *视觉增强*：退场伴随轻微等比微缩（1.00 -> 0.96）。

    #v(4pt)
    *2. Uncreate（几何逆向收缩擦除）*
    - *数学核心*：Manim `Uncreate` 逆转逻辑；
    - *视觉呈现*：场景由内向外消解，边缘留存高光线框并收拢消失。
  ],
  card-box(title: "进入动效清单 (Entrance, N = 2)", icon: "✨", stroke-color: rgb("#16a34a"), fill-color: rgb("#f0fdf4"))[
    *1. FadeIn（动力学减速冲入）*
    - *数学核心*：Manim `rush_from`（快速冲入，阻尼刹车）；
    - *视觉增强*：轻微缩放回弹（1.04 -> 1.00）落地感。

    #v(4pt)
    *2. Create / DrawBorderThenFill（边框勾勒填充）*
    - *数学核心*：前半程沿矩形周长绘制线框，后半程注入纹理；
    - *视觉呈现*：光笔勾勒轮廓，极具科技与数学板书感。
  ]
)

#v(4pt)
=== 组合体验矩阵 ($2 times 2$)

#table(
  columns: (1.2fr, 1fr, 1.2fr, 2.4fr),
  align: (center + horizon, center + horizon, center + horizon, left + horizon),
  fill: (x, y) => if y == 0 { rgb("#f1f5f9") } else if calc.even(y) { rgb("#f8fafc") } else { none },
  stroke: 0.5pt + c-border,
  [#text(weight: "bold")[组合名称]], [#text(weight: "bold")[Exit 选型]], [#text(weight: "bold")[Entrance 选型]], [#text(weight: "bold")[适用场景与视觉风格]],
  [*Kinetic Dissolve*], [FadeOut], [FadeIn], [日常高频切换、PPT 翻页；比原生淡入淡出更紧凑高级],
  [*Construct In*], [FadeOut], [Create], [切到主舞台/代码窗口；旧画面利落退场，新画面高光构建],
  [*Deconstruct Out*], [Uncreate], [FadeIn], [章节结束、切回聊天大厅；解构退场，新画面柔和承接],
  [*Full Manim*], [Uncreate], [Create], [*核心招牌效果*：旧画面线条化瓦解，新画面光线重组诞生]
)

== 4. 技术可行性与难点分析：为何 v0.1.0 先不做 `Write`？

- *`Create` 容易做好*：OBS 中所有场景与窗口捕获在 GPU 视口内都是标准的四边形矩形（Quad Texture）。在 HLSL 像素着色器中，矩形周长和内外边界距离的参数化计算非常精确、开销极低。
- *`Write` 的工程瓶颈*：OBS 的文字来源（如 FreeType2、GDI+ 文字源）提交到渲染管道时，已经烘焙为*光栅化位图纹理（RGBA）*，缺失文字笔画内部的贝塞尔骨架与笔顺拓扑。
  - 要在 OBS 里做出真正的逐笔手写，需引入矢量解析引擎（HarfBuzz / FreeType Outline）并在 GPU 维护矢量路径，这会极大增加初期的研发复杂度。
  - *开发策略*：0.1.0 坚决不碰矢量笔画解析，集中精力打磨矩形与画面级别的 `Create` 与动力学缓动。

== 5. Manim 源代码映射与数学移植计划

#table(
  columns: (1.2fr, 1.8fr, 2.5fr),
  align: (center + horizon, left + horizon, left + horizon),
  fill: (x, y) => if y == 0 { rgb("#f1f5f9") } else if calc.even(y) { rgb("#f8fafc") } else { none },
  stroke: 0.5pt + c-border,
  [#text(weight: "bold")[模块名称]], [#text(weight: "bold")[Manim 原始代码路径]], [#text(weight: "bold")[移植与实现方案]],
  [*缓动函数*], [`manim/utils/rate_functions.py`], [纯数学 C++ 头文件 `src/math/rate_functions.hpp`，提供 `smooth`、`double_smooth`、`rush_into`、`rush_from`、`exponential_decay`],
  [*边框勾勒与填充*], [`manim/animation/creation.py` (`DrawBorderThenFill`)], [转换为 HLSL Shader：`data/effects/manim_border_fill.effect`],
  [*动力学退隐*], [`manim/animation/fading.py` (`FadeIn`, `FadeOut`)], [矩阵变换 + Alpha 插值着色器：`data/effects/manim_kinetic_fade.effect`]
)

== 6. 构建与工程方案规范

- *开发语言*：现代 C++ (C++20) + `libobs` C 接口 + HLSL 着色器。
- *平台支持*：*仅支持 Windows x64 与 Linux x64*（彻底剔除 macOS 以免除通用二进制和证书签名的繁琐负担）。
- *编译与构建策略*：*云端构建（No Local Builds）*
  - 本地不安装冗杂的 OBS SDK、Qt 或构建工具链；
  - 本地只负责编辑源码、数学逻辑和着色器；
  - 通过 GitHub Actions 云端流水线（`windows-latest` + `ubuntu-24.04`）自动编译；
  - 触发 push 时，云端输出打包好的 `.dll` / `.so` 压缩包（Artifact），本地下载直接放入 OBS `obs-plugins/64bit` 即可实测。

== 7. 后续实施阶段规划

#grid(
  columns: (1fr, 1fr, 1fr, 1fr),
  gutter: 8pt,
  card-box(title: "Phase 1: 脚手架", stroke-color: c-primary)[
    配置基础 `.github/workflows/build.yml` 与 `CMakeLists.txt`。
  ],
  card-box(title: "Phase 2: 数学层", stroke-color: c-accent)[
    编写纯头文件 `rate_functions.hpp` 并配合单元测试。
  ],
  card-box(title: "Phase 3: 着色器层", stroke-color: rgb("#ea580c"))[
    编写 `border_fill.effect`，验证 GPU 沿矩形轮廓周长画高光。
  ],
  card-box(title: "Phase 4: 插件接入", stroke-color: rgb("#16a34a"))[
    注册 `obs_source_info` 转场插件，提供 OBS 属性下拉菜单与滑块。
  ]
)

#v(8pt)
#align(center)[
  #text(size: 9pt, fill: c-muted)[OBS Manim-Like Motion · 让每一次场景切换都兼具数学美感与动效张力]
]
