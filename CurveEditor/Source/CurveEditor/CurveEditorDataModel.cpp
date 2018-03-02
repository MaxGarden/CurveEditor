#include "pch.h"
#include "CurveEditorDataModel.h"

SEditorStyle& CCurveEditorDataModel::GetStyle() noexcept
{
    return m_EditorStyle;
}

const SEditorStyle& CCurveEditorDataModel::GetStyle() const noexcept
{
    return m_EditorStyle;
}