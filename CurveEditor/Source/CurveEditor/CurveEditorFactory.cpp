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

    result->AddViewComponent(std::make_unique<CCurveEditorBackgroundViewComponent>(*result), EComponentOrder::Background);
    result->AddViewComponent(std::make_unique<CCurveEditorGridViewComponent>(*result), EComponentOrder::Background);

    result->AddViewComponent(std::make_unique<CCurveEditorSplinesViewComponent>(*result, m_SplineViewFactory), EComponentOrder::Splines);

    result->AddViewComponent(std::make_unique<CCurveEditorDebugComponent>(*result, m_EditorContext), EComponentOrder::Foreground);

    result->AddViewComponent(std::make_unique<CCurveEditorToolHandlerComponent>(*result), EComponentOrder::Foreground);

    if (!result->Initialize())
        return nullptr;

    return std::move(result);
}