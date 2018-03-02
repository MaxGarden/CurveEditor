#include "pch.h"
#include "CurveEditorView.h"
#include "CurveEditorViewComponent.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"

void CCurveEditorView::OnFrame()
{
    for (const auto& component : m_Components)
    {
        if (component)
            component->OnFrame();
    }
}

bool CCurveEditorView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if (!controller)
    {
        m_Controller.reset();
        return true;
    }

    const auto curveEditorController = std::dynamic_pointer_cast<CCurveEditorController>(controller);
    if (!curveEditorController)
        return false;

    m_Controller = std::move(curveEditorController);
    return true;
}

bool CCurveEditorView::SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept
{
    if (!dataModel)
    {
        m_DataModel.reset();
        return true;
    }

    const auto curveEditorDataModel = std::dynamic_pointer_cast<const CCurveEditorDataModel>(dataModel);
    if (!curveEditorDataModel)
        return false;

    m_DataModel = std::move(curveEditorDataModel);
    return true;
}

CEditorCanvas& CCurveEditorView::GetCanvas() noexcept
{
    return m_Canvas;
}

const CEditorCanvas& CCurveEditorView::GetCanvas() const noexcept
{
    return m_Canvas;
}

const CCurveEditorDataModelConstSharedPtr& CCurveEditorView::GetDataModel() const noexcept
{
    return m_DataModel;
}

const CCurveEditorControllerSharedPtr& CCurveEditorView::GetController() const noexcept
{
    return m_Controller;
}

bool CCurveEditorView::AddComponent(ICurveEditorViewComponentUniquePtr&& component)
{
    if (!component)
        return false;

    m_Components.emplace_back(std::move(component));
    return true;
}