#include "pch.h"
#include "CurveEditorView.h"
#include "CurveEditorViewComponent.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"

void CCurveEditorViewBase::OnFrame()
{
    //to override
}

bool CCurveEditorViewBase::SetController(const IEditorControllerSharedPtr& controller) noexcept
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

bool CCurveEditorViewBase::SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept
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

const CCurveEditorDataModelConstSharedPtr& CCurveEditorViewBase::GetDataModel() const noexcept
{
    return m_DataModel;
}

const CCurveEditorControllerSharedPtr& CCurveEditorViewBase::GetController() const noexcept
{
    return m_Controller;
}


void CCurveEditorView::OnFrame()
{
    //only for tests
    //begin
    m_Canvas.GetWindowCanvas() = CWindowCanvas(ImGui::GetWindowPos(), ImGui::GetWindowSize(), ImVec2(1, 1), ImGui::GetWindowSize());
    //end

    VisitViews([](auto& view)
    {
        view.OnFrame();
    });
}

bool CCurveEditorView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if (!CCurveEditorViewBase::SetController(controller))
        return false;

    auto result = true;
    VisitViews([&controller, &result](auto& view)
    {
        result &= view.SetController(controller);
    });

    EDITOR_ASSERT(result && "Views should accept new controller if main view accepts.");

    return true;
}

bool CCurveEditorView::SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept
{
    if (!CCurveEditorViewBase::SetDataModel(dataModel))
        return false;

    auto result = true;
    VisitViews([&dataModel, &result](auto& view)
    {
        result &= view.SetDataModel(dataModel);
    });

    EDITOR_ASSERT(result && "Views should accept new data model if main view accepts.");

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

bool CCurveEditorView::AddView(CCurveEditorViewBaseUniquePtr&& view)
{
    if (!view)
        return false;

    auto isValid = true;
    isValid &= view->SetController(GetController());
    isValid &= view->SetDataModel(GetDataModel());

    if (!isValid)
        return false;

    m_Views.emplace_back(std::move(view));
    return true;
}

void CCurveEditorView::VisitViews(const std::function<void(CCurveEditorViewBase&)>& visitor) noexcept
{
    if (!visitor)
        return;

    for (const auto& view : m_Views)
    {
        if (view)
            visitor(*view);
    }
}