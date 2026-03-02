Feature: WidgetFocusManager
  为了提供可预测的键盘和编程焦点行为
  作为 UI 框架
  WidgetFocusManager 应一致且可访问地管理控件焦点

  Background:
    假设有多个受 WidgetFocusManager 管理的可聚焦控件

  场景: 新控件创建时设置初始焦点
    假设新控件 "A" 被添加到管理器
    当控件请求初始焦点
    那么 WidgetFocusManager 应将 "A" 标记为已聚焦
    并且聚焦控件应发出 "focusGained" 事件

  场景: 使用 Tab 向前移动焦点
    假设控件 "A", "B", "C" 按 Tab 顺序可聚焦
    且当前 "A" 处于聚焦状态
    当用户按下 "Tab"
    那么焦点应移动到 "B"
    并为 "A" 发送 "focusLost" 事件，为 "B" 发送 "focusGained" 事件

  场景: 使用 Shift+Tab 向后移动焦点
    假设控件 "A", "B", "C" 按 Tab 顺序可聚焦
    且当前 "C" 处于聚焦状态
    当用户按下 "Shift+Tab"
    那么焦点应移动到 "B"

  场景: 程序化焦点切换
    假设存在控件 "A" 和 "B"
    当应用调用 focus("B")
    那么 "B" 应立即获得焦点

  场景: 模态对话框捕获焦点
    假设模态控件 "Modal" 打开并标记为模态
    且模态内有可聚焦控件 "M1", "M2"
    当 "Modal" 为活动状态
    那么 Tab 与 Shift+Tab 应仅在 "M1" 和 "M2" 之间循环
    并且焦点不应移出模态范围

  场景: 控件移除后的焦点恢复
    假设 "B" 当前被聚焦且控件 "B" 被移除
    当移除完成后
    那么焦点应移动到按 Tab 顺序的最近可聚焦控件
    或在无可用控件时清除焦点

  场景: 焦点可见性与无障碍通知
    假设控件通过键盘获得焦点
    那么 WidgetFocusManager 应为该控件设置焦点可见状态
    并通过无障碍接口通知焦点变化
