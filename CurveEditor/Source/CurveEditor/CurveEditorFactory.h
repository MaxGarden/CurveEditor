#pragma  once
#if !defined(__CURVE_EDITOR_FACTORY_H__)

#include "CurveEditorView.h"
#include "SplineView.h"

class CCurveEditorViewFactory final : public IEditorViewFactory
{
public:
    CCurveEditorViewFactory(ICurveEditorSplineViewFactory& splineViewFactory);
    virtual ~CCurveEditorViewFactory() override final = default;

    virtual IEditorViewUniquePtr Create() override final;

private:
    ICurveEditorSplineViewFactory& m_SplineViewFactory;
};

#endif //__CURVE_EDITOR_FACTORY_H__