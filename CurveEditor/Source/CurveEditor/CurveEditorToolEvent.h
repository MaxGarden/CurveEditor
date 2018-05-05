#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_EVENT_H__)

#include <ax/ax.h>

enum class ECurveEditorMouseButton
{
    Left,
    Right,
    Middle,
    User1,
    User2,

    __Count
};

enum class ECurveEditorModifier
{
    Control,
    Alt,
    Shift,
    Super,

    _Count
};

class CCurveEditorToolEvent
{
public:
    CCurveEditorToolEvent(ICurveEditorView& editorView);
    virtual ~CCurveEditorToolEvent() = default;

    ICurveEditorView& GetEditorView() const noexcept;

private:
    ICurveEditorView& m_EditorView;
};

class CCurveEditorToolMouseEvent : public CCurveEditorToolEvent
{
public:
    CCurveEditorToolMouseEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition);
    virtual ~CCurveEditorToolMouseEvent() override = default;

    const ax::pointf& GetMousePosition() const noexcept;

private:
    const ax::pointf m_MousePosition;
};

class CCurveEditorToolMouseWheelEvent final : public CCurveEditorToolMouseEvent
{
public:
    CCurveEditorToolMouseWheelEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, float wheelValue);
    virtual ~CCurveEditorToolMouseWheelEvent() override final = default;

    float GetWheelValue() const noexcept;

private:
    float m_WheelValue;
};

class CCurveEditorToolMouseButtonEvent : public CCurveEditorToolMouseEvent
{
public:
    CCurveEditorToolMouseButtonEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton);
    virtual ~CCurveEditorToolMouseButtonEvent() override = default;

    ECurveEditorMouseButton GetMouseButton() const noexcept;

private:
    const ECurveEditorMouseButton m_MouseButton;
};

class CCurveEditorToolMouseDragEvent final : public CCurveEditorToolMouseButtonEvent
{
public:
    CCurveEditorToolMouseDragEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton, const ax::pointf& dragDelta);
    virtual ~CCurveEditorToolMouseDragEvent() override final = default;

    const ax::pointf& GetDragDelta() const noexcept;

private:
    const ax::pointf m_DragDelta;
};

class CCurveEditorToolModifierEvent final : public CCurveEditorToolEvent
{
public:
    CCurveEditorToolModifierEvent(ICurveEditorView& editorView, ECurveEditorModifier modifier);
    virtual ~CCurveEditorToolModifierEvent() override final = default;

    ECurveEditorModifier GetModifier() const noexcept;

private:
    const ECurveEditorModifier m_Modifier;
};

#endif //__CURVE_EDITOR_TOOL_EVENT_H__