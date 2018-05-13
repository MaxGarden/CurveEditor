#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_VIEW_H__)

#include "EditorView.h"
#include "SplineDataModel.h"

using SplineViewGetter = std::function<ICurveEditorSplineView*(const SplineID& splineID)>;

class ICurveEditorSelectionView : public IEditorView
{
public:
    virtual ~ICurveEditorSelectionView() override = default;

    virtual bool SetSplineViewGetter(SplineViewGetter splineViewGetter) = 0;

    static ICurveEditorSelectionViewUniquePtr Create();
};

#endif //__CURVE_EDITOR_SELECTION_VIEW_H__