# WidgetFocusManager

## 概述

WidgetFocusManager 提供了一组用于管理 UI 中键盘与程序化焦点行为的规范与测试场景。本文件夹包含以 Gherkin 编写的 feature 文件，描述预期的焦点行为，包括：

- Tab 顺序导航
- 程序化聚焦
- 模态焦点捕获
- 移除后焦点恢复
- 无障碍通知

## 文件

- `WidgetFocusManager.en.feature` — WidgetFocusManager 的英文 Gherkin 场景文件
- `WidgetFocusManager.zh.feature` — WidgetFocusManager 的中文 Gherkin 场景文件

## 目标

- 为焦点处理定义清晰、可测试的行为
- 支持键盘导航（`Tab` / `Shift+Tab`）与程序化焦点变更
- 确保模态对话框在激活时捕获焦点
- 在控件被移除时恢复或清除焦点
- 在焦点变化时提供 `focus-visible` 状态并通知无障碍 API

## 使用方法

1. 将 `.feature` 文件作为实现或测试焦点管理器的规范。
2. 将 `.feature` 文件导入 BDD/测试框架（如 Cucumber、SpecFlow），并实现对应的 step definitions 来驱动 UI 或 `WidgetFocusManager` 的实现。

## 实现要点

- 焦点顺序应遵循确定性的 Tab 顺序；组件需能够声明自己是否可聚焦。
- 通过键盘获得的焦点应设置 `focus-visible` 状态，以便样式显示键盘焦点。
- 模态容器应在激活时限制 `Tab` / `Shift+Tab` 在其可聚焦子项之间循环。
- 当被聚焦的控件被移除时，应将焦点移动到下一个合理的控件，或在无可用控件时清除焦点。
- 发出明确的焦点事件（`focusGained` / `focusLost`），并与无障碍接口集成。

## 贡献

如新增场景或功能，请同时更新英文与中文的 feature 文件及此 README，且保持场景简洁明确。

## 许可

请遵循仓库中定义的许可协议；若仓库无许可，请在复用前联系项目负责人。
