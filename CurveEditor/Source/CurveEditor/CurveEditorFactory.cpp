#include "pch.h"
#include "CurveEditorFactory.h"
#include "CurveEditorDataModel.h"
#include "Components/BackgroundComponent.h"
#include "Components/GridComponent.h"

std::unique_ptr<IEditorView> CCurveEditorViewFactory::Create()
{
    auto result = std::make_unique<CCurveEditorView>();

    result->AddComponent(std::make_unique<CCurveEditorBackgroundViewComponent>(*result));
    result->AddComponent(std::make_unique<CCurveEditorGridViewComponent>(*result));

    return std::move(result);
}