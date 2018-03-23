#include "pch.h"
#include "CurveEditorViewDataModel.h"

CEditorCanvas& CCurveEditorViewDataModel::GetEditorCanvas() noexcept
{
    return m_EditorCanvas;
}

const CEditorCanvas& CCurveEditorViewDataModel::GetEditorCanvas() const noexcept
{
    return m_EditorCanvas;
}