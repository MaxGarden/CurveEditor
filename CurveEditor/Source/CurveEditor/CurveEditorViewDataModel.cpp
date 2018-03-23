#include "pch.h"
#include "CurveEditorViewDataModel.h"

class CCurveEditorViewDataModel final : public ICurveEditorViewDataModel
{
public:
    CCurveEditorViewDataModel() = default;
    virtual ~CCurveEditorViewDataModel() override final = default;

    virtual CEditorCanvas& GetEditorCanvas() noexcept override final;
    virtual const CEditorCanvas& GetEditorCanvas() const noexcept override final;

private:
    CEditorCanvas m_EditorCanvas = CEditorCanvas({ 100.0f, 100.0f });
};

CEditorCanvas& CCurveEditorViewDataModel::GetEditorCanvas() noexcept
{
    return m_EditorCanvas;
}

const CEditorCanvas& CCurveEditorViewDataModel::GetEditorCanvas() const noexcept
{
    return m_EditorCanvas;
}

ICurveEditorViewDataModelUniquePtr ICurveEditorViewDataModel::Create()
{
    return std::make_unique<CCurveEditorViewDataModel>();
}