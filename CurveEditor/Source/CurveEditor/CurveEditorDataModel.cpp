#include "pch.h"
#include "CurveEditorDataModel.h"

class CCurveEditorDataModel final : public CEditorListenableBase<ICurveEditorDataModel, ICurveEditorDataModelListener>
{
public:
    CCurveEditorDataModel() = default;
    virtual ~CCurveEditorDataModel() override final = default;

    virtual void SetStyle(SCurveEditorStyle&& style) override final;
    virtual const SCurveEditorStyle& GetStyle() const noexcept override final;

    virtual ICurveEditorSplineDataModelSharedPtr AddSplineDataModel(const SplineColor& color) override final;
    virtual bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;

    virtual const ICurveEditorSplineDataModelSharedPtr& GetSplineDataModel(const SplineID& id) const noexcept override final;
    virtual void VisitSplineDataModels(const ConstVisitorType<ICurveEditorSplineDataModelSharedPtr>& visitor) const noexcept override final;

private:
    SplineID GenerateFreeSplineID() const noexcept;

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

ICurveEditorSplineDataModelSharedPtr CCurveEditorDataModel::AddSplineDataModel(const SplineColor& color)
{
    auto splineDataModel = ICurveEditorSplineDataModel::Create(GenerateFreeSplineID(), color);
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

const ICurveEditorSplineDataModelSharedPtr& CCurveEditorDataModel::GetSplineDataModel(const SplineID& id) const noexcept
{
    static const ICurveEditorSplineDataModelSharedPtr null;

    const auto iterator = std::find_if(m_SplinesDataModels.cbegin(), m_SplinesDataModels.cend(), [&id](const auto& splineDataModel)
    {
        return splineDataModel && splineDataModel->GetID() == id;
    });

    if(iterator == m_SplinesDataModels.cend())
        return null;

    return *iterator;
}

void CCurveEditorDataModel::VisitSplineDataModels(const ConstVisitorType<ICurveEditorSplineDataModelSharedPtr>& visitor) const noexcept
{
    VisitObjectsContainer(m_SplinesDataModels, visitor);
}

SplineID CCurveEditorDataModel::GenerateFreeSplineID() const noexcept
{
    while (true)
    {
        const auto result = ICurveEditorSplineDataModel::GenerateSplineID();
        if (!GetSplineDataModel(result))
            return result;
    }
}

ICurveEditorDataModelUniquePtr ICurveEditorDataModel::Create()
{
    return std::make_unique<CCurveEditorDataModel>();
}