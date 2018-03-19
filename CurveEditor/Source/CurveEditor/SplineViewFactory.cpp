#include "pch.h"
#include "SplineViewFactory.h"

ICurveEditorSplineViewUniquePtr CCurveEditorSplineViewFactory::Create(CCurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr&)
{
    return std::make_unique<CCurveEditorSplineView>(editorView);
}