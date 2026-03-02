Feature: WidgetFocusManager
  In order to provide predictable keyboard and programmatic focus behavior
  As a UI framework
  The WidgetFocusManager should manage focus for widgets consistently and accessibly

  Background:
    Given a UI with multiple focusable widgets managed by WidgetFocusManager

  Scenario: Set initial focus when a widget is created
    Given a new widget "A" is added to the manager
    When the widget requests initial focus
    Then WidgetFocusManager should mark "A" as focused
    And the focused widget should emit a "focusGained" event

  Scenario: Move focus forward with Tab
    Given widgets "A", "B", "C" are focusable in tab order
    And "A" is currently focused
    When the user presses "Tab"
    Then focus should move to "B"
    And a "focusLost" event should be emitted for "A"
    And a "focusGained" event should be emitted for "B"

  Scenario: Move focus backward with Shift+Tab
    Given widgets "A", "B", "C" are focusable in tab order
    And "C" is currently focused
    When the user presses "Shift+Tab"
    Then focus should move to "B"

  Scenario: Programmatic focus change
    Given widgets "A" and "B" exist
    When the application calls focus("B")
    Then "B" should become focused immediately

  Scenario: Modal dialog traps focus
    Given a modal widget "Modal" is opened and marked as modal
    And modal contains focusable widgets "M1", "M2"
    When "Modal" is active
    Then Tab and Shift+Tab should cycle focus only between "M1" and "M2"
    And focus should not move to widgets outside the modal

  Scenario: Focus restoration after widget removal
    Given "B" is focused and widget "B" is removed
    When removal completes
    Then focus should move to the nearest focusable widget according to tab order
    Or WidgetFocusManager should clear focus if none available

  Scenario: Focus-visible and accessibility notifications
    Given a widget becomes focused via keyboard
    Then WidgetFocusManager should set a focus-visible state for that widget
    And accessibility APIs should be notified of focus changes
