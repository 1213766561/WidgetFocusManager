# WidgetFocusManager

## Overview

WidgetFocusManager provides rules and test scenarios for managing keyboard and programmatic focus in a UI. This folder contains Gherkin feature files that describe expected focus behavior, including:

- Tab order navigation
- Programmatic focus
- Modal focus trapping
- Focus restoration
- Accessibility notifications

## Files

- `WidgetFocusManager.en.feature` — English Gherkin feature scenarios for WidgetFocusManager
- `WidgetFocusManager.zh.feature` — Chinese Gherkin feature scenarios for WidgetFocusManager

## Goals

- Define clear, testable behavior for focus handling
- Support keyboard navigation (`Tab` / `Shift+Tab`) and programmatic focus changes
- Ensure modal dialogs trap focus while active
- Restore or clear focus when widgets are removed
- Provide `focus-visible` state and notify accessibility APIs on focus changes

## Usage

1. Use the `.feature` files as the specification for implementing or testing a focus manager.
2. Import the `.feature` files into your BDD/test runner (e.g., Cucumber, SpecFlow) and implement step definitions that exercise the UI or the `WidgetFocusManager` implementation.

## Implementation notes

- Focus order should follow a deterministic tab order; components must expose whether they are focusable.
- Keyboard-originated focus should set a `focus-visible` state so styles can show keyboard focus.
- Modal containers should restrict `Tab` / `Shift+Tab` to their focusable descendants while active.
- When a focused widget is removed, move focus to the next logical widget or clear it if none exist.
- Emit explicit focus events (`focusGained` / `focusLost`) and integrate with accessibility APIs.

## Contributing

If you add features or scenarios, update both the English and Chinese feature files and this README. Keep scenarios small and focused.

## License

Follow the repository's license. If none exists, consult the project owner before reuse.
