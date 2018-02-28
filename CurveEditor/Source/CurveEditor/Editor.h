#pragma  once
#if !defined(__EDITOR_H__)

#include "Canvas.h"
#include "EditorView.h"
#include "Style.h"

class CCurveEditorView final : public CEditorView
{
public:
    CCurveEditorView() = default;
    virtual ~CCurveEditorView() override final = default;

    CEditorCanvas& GetCanvas() noexcept;
    const CEditorCanvas& GetCanvas() const noexcept;

    const SEditorStyle& GetStyle() const noexcept;

private:
    CEditorCanvas m_Canvas = CEditorCanvas(ImVec2(100.0f, 100.0f));
    SEditorStyle m_EditorStyle; //todo: move to editor data
};

class CCurveEditorViewComponent : public IEditorViewComponent
{
public:
    CCurveEditorViewComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorViewComponent() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList);

    const CCurveEditorView& GetEditorView() const noexcept;

private:
    const CCurveEditorView& m_EditorView;
};

#endif //__EDITOR_H__