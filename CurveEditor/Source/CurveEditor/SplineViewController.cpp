#include "pch.h"
#include "CurveEditorViewController.h"

bool CCurveEditorViewController::SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept
{
    if (!dataModel)
    {
        m_DataModel.reset();
        OnDataModelChanged();
        return true;
    }

    const auto editorViewDataModel = std::dynamic_pointer_cast<CCurveEditorViewDataModel>(dataModel);
    if (!editorViewDataModel)
        return false;

    m_DataModel = std::move(editorViewDataModel);
    OnDataModelChanged();

    return true;
}

void CCurveEditorViewController::OnDataModelChanged() noexcept
{
    //TODO
}