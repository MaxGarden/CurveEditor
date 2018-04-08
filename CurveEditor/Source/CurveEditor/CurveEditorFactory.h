#pragma  once
#if !defined(__CURVE_EDITOR_FACTORY_H__)

#include "CurveEditorView.h"
#include "SplineView.h"

class CCurveEditorViewFactory final : public IEditorViewFactory
{
public:
    CCurveEditorViewFactory(IEditorContext& editorContext, ICurveEditorSplineViewFactory& splineViewFactory);
    virtual ~CCurveEditorViewFactory() override final = default;

    virtual IEditorViewUniquePtr Create() override final;

private:
    IEditorContext& m_EditorContext;
    ICurveEditorSplineViewFactory& m_SplineViewFactory;
};

#endif //__CURVE_EDITOR_FACTORY_H__