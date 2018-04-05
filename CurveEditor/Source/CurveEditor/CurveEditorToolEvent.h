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

class CCurveEditorToolEvent
{
public:
    CCurveEditorToolEvent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorToolEvent() = default;

    const CCurveEditorView& GetEditorView() const noexcept;

private:
    const CCurveEditorView& m_EditorView;
};

class CCurveEditorToolWheelEvent : public CCurveEditorToolEvent
{
public:
    CCurveEditorToolWheelEvent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorToolWheelEvent() override = default;
};

class CCurveEditorToolMouseEvent : public CCurveEditorToolEvent
{
public:
    CCurveEditorToolMouseEvent(const CCurveEditorView& editorView, const ax::pointf& mousePosition);
    virtual ~CCurveEditorToolMouseEvent() override = default;

    const ax::pointf& GetMousePosition() const noexcept;

private:
    const ax::pointf m_MousePosition;
};

class CCurveEditorToolMouseButtonEvent : public CCurveEditorToolMouseEvent
{
public:
    CCurveEditorToolMouseButtonEvent(const CCurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton);
    virtual ~CCurveEditorToolMouseButtonEvent() override = default;

    ECurveEditorMouseButton GetMouseButton() const noexcept;

private:
    const ECurveEditorMouseButton m_MouseButton;
};

class CCurveEditorToolKeyEvent : public CCurveEditorToolEvent
{
public:
    CCurveEditorToolKeyEvent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorToolKeyEvent() override = default;
};

#endif //__CURVE_EDITOR_TOOL_EVENT_H__