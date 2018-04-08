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
    auto result = std::make_unique<CCurveEditorView>(m_SplineViewFactory);

    result->AddView(std::make_unique<CCurveEditorBackgroundViewComponent>(*result));
    result->AddView(std::make_unique<CCurveEditorGridViewComponent>(*result));

    result->AddView(std::make_unique<CCurveEditorDebugComponent>(*result, m_EditorContext));

    result->AddView(std::make_unique<CCurveEditorToolHandlerComponent>(*result));

    return std::move(result);
}