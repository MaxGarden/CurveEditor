#include "pch.h"
#include "CurveEditorFactory.h"
#include "CurveEditorDataModel.h"
#include "Components/BackgroundComponent.h"
#include "Components/GridComponent.h"
#include "Components/DebugComponent.h"
#include "Components/ToolHandlerComponent.h"
#include "Components/SplinesComponent.h"

CCurveEditorViewFactory::CCurveEditorViewFactory(IEditorContext& editorContext, ICurveEditorSplineViewFactory& splineViewFactory) :
    m_EditorContext(editorContext),
    m_SplineViewFactory(splineViewFactory)
{
}

IEditorViewUniquePtr CCurveEditorViewFactory::Create()
{
    auto result = ICurveEditorView::Create();

    result->AddViewComponent(ICurveEditorBackgroundViewComponent::Create(*result), EComponentOrder::Background);
    result->AddViewComponent(ICurveEditorGridViewComponent::Create(*result), EComponentOrder::Background);

    result->AddViewComponent(ICurveEditorSplinesViewComponent::Create(*result, m_SplineViewFactory), EComponentOrder::Splines);

    result->AddViewComponent(ICurveEditorDebugComponent::Create(*result, m_EditorContext), EComponentOrder::Foreground);

    result->AddViewComponent(ICurveEditorToolHandlerComponent::Create(*result), EComponentOrder::Foreground);

    if (!result->Initialize())
        return nullptr;

    return std::move(result);
}