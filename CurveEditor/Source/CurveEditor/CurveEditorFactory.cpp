#include "pch.h"
#include "CurveEditorFactory.h"
#include "CurveEditorDataModel.h"
#include "Components/BackgroundComponent.h"
#include "Components/GridComponent.h"
#include "Components/DebugComponent.h"
#include "Components/ToolHandlerComponent.h"

CCurveEditorViewFactory::CCurveEditorViewFactory(IEditorContext& editorContext, ICurveEditorSplineViewFactory& splineViewFactory) :
    m_EditorContext(editorContext),
    m_SplineViewFactory(splineViewFactory)
{
}

IEditorViewUniquePtr CCurveEditorViewFactory::Create()
{
    auto result = ICurveEditorView::Create(m_SplineViewFactory);

    result->AddViewComponent(std::make_unique<CCurveEditorBackgroundViewComponent>(*result));
    result->AddViewComponent(std::make_unique<CCurveEditorGridViewComponent>(*result));

    result->AddViewComponent(std::make_unique<CCurveEditorDebugComponent>(*result, m_EditorContext));

    result->AddViewComponent(std::make_unique<CCurveEditorToolHandlerComponent>(*result));

    return std::move(result);
}