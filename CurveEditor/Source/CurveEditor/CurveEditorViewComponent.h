#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_COMPONENT_H__)

#include "CurveEditorView.h"
#include "CurveEditorDataModel.h"

class ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorViewComponent() = default;

    virtual void OnFrame() = 0;
};

class CCurveEditorViewComponentBase : public ICurveEditorViewComponent
{
public:
    CCurveEditorViewComponentBase(const CCurveEditorView& editorView);
    virtual ~CCurveEditorViewComponentBase() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, const CCurveEditorDataModel& dataModel);

    const CCurveEditorView& GetEditorView() const noexcept;

private:
    const CCurveEditorView& m_EditorView;
};

#endif //__CURVE_EDITOR_VIEW_COMPONENT_H__