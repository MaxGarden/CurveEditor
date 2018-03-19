#include "pch.h"
#include "CurveEditorFactory.h"
#include "CurveEditorDataModel.h"
#include "Components/BackgroundComponent.h"
#include "Components/GridComponent.h"
#include "Components/DebugComponent.h"

CCurveEditorViewFactory::CCurveEditorViewFactory(ICurveEditorSplineViewFactory& splineViewFactory) :
    m_SplineViewFactory(splineViewFactory)
{
}

IEditorViewUniquePtr CCurveEditorViewFactory::Create()
{
    auto result = std::make_unique<CCurveEditorView>(m_SplineViewFactory);

    result->AddView(std::make_unique<CCurveEditorBackgroundViewComponent>(*result));
    result->AddView(std::make_unique<CCurveEditorGridViewComponent>(*result));

	result->AddView(std::make_unique<CCurveEditorDebugComponent>(*result));

    return std::move(result);
}