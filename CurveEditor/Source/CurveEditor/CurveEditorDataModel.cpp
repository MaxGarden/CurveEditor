#include "pch.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"

class CCurveEditorDataModel final : public CEditorListenableBase<ICurveEditorDataModel, ICurveEditorDataModelListener>
{
public:
    CCurveEditorDataModel() = default;
    virtual ~CCurveEditorDataModel() override final = default;

    virtual void SetStyle(SCurveEditorStyle&& style) override final;
    virtual const SCurveEditorStyle& GetStyle() const noexcept override final;

    virtual ICurveEditorSplineDataModelSharedPtr AddSplineDataModel(std::string_view name, unsigned int color) override final;
    virtual bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;

    virtual const std::vector<ICurveEditorSplineDataModelSharedPtr>& GetSplinesDataModels() const noexcept override final;

private:
    SCurveEditorStyle m_EditorStyle;
    std::vector<ICurveEditorSplineDataModelSharedPtr> m_SplinesDataModels;
};

void CCurveEditorDataModel::SetStyle(SCurveEditorStyle&& style)
{
    m_EditorStyle = std::move(style);
    NotifyListeners(&ICurveEditorDataModelListener::OnStyleChanged, m_EditorStyle);
}

const SCurveEditorStyle& CCurveEditorDataModel::GetStyle() const noexcept
{
    return m_EditorStyle;
}

ICurveEditorSplineDataModelSharedPtr CCurveEditorDataModel::AddSplineDataModel(std::string_view name, unsigned int color)
{
    auto splineDataModel = ICurveEditorSplineDataModel::Create(name, color);
    if (!splineDataModel)
        return nullptr;

    m_SplinesDataModels.emplace_back(std::move(splineDataModel));
    const auto& addedSplineDataModel = m_SplinesDataModels.back();

    NotifyListeners(&ICurveEditorDataModelListener::OnSplineCreated, addedSplineDataModel);
    return addedSplineDataModel;
}

bool CCurveEditorDataModel::RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    if (RemoveFromContainer(m_SplinesDataModels, splineDataModel))
    {
        NotifyListeners(&ICurveEditorDataModelListener::OnSplineDestroyed, splineDataModel);
        return true;
    }

    return false;
}

const std::vector<ICurveEditorSplineDataModelSharedPtr>& CCurveEditorDataModel::GetSplinesDataModels() const noexcept
{
    return m_SplinesDataModels;
}

ICurveEditorDataModelUniquePtr ICurveEditorDataModel::Create()
{
    return std::make_unique<CCurveEditorDataModel>();
}