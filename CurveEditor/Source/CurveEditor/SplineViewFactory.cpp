#include "pch.h"
#include "SplineViewFactory.h"

ICurveEditorSplineViewUniquePtr CCurveEditorSplineViewFactory::Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr&)
{
    return std::make_unique<CCurveEditorSplineView>(editorView);
}