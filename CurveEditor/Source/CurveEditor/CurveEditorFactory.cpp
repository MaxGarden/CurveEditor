#include "pch.h"
#include "CurveEditorFactory.h"
#include "CurveEditorDataModel.h"
#include "Components/BackgroundComponent.h"
#include "Components/GridComponent.h"
#include "Components/DebugComponent.h"
#include "Components/ToolHandlerComponent.h"
#include "Components/SplinesComponent.h"
#include "Components/SelectionComponent.h"
#include "Components/NavigationComponent.h"
#include "Components/RulerComponent.h"

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

    result->AddViewComponent(ICurveEditorRulerViewComponent::Create(*result), EComponentOrder::Foreground);

    result->AddViewComponent(ICurveEditorDebugComponent::Create(*result, m_EditorContext), EComponentOrder::Foreground);

    result->AddViewComponent(ICurveEditorSelectionViewComponent::Create(*result), EComponentOrder::Selection);

    result->AddViewComponent(ICurveEditorToolHandlerComponent::Create(*result), EComponentOrder::ToolHandler);
    result->AddViewComponent(ICurveEditorNavigationComponent::Create(*result), EComponentOrder::Background);

    if (!result->Initialize())
        return nullptr;

    return std::move(result);
}