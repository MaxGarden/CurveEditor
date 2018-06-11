#include "pch.h"
#include "CurveEditorDataModel.h"
#include "EditorListenableBase.h"
#include "CurveEditorSelectionDataModel.h"

class CCurveEditorDataModel final : public CEditorListenableBase<ICurveEditorDataModel, ICurveEditorDataModelListener>
{
public:
    CCurveEditorDataModel() = default;
    virtual ~CCurveEditorDataModel() override final;

    virtual void SetStyle(SCurveEditorStyle&& style) override final;
    virtual const SCurveEditorStyle& GetStyle() const noexcept override final;

    virtual SplineID GetFreeSplineID() const noexcept override final;

    virtual bool AddSplineDataModel(ICurveEditorSplineDataModelUniquePtr&& splineDataModel) override final;
    virtual bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;

    virtual const ICurveEditorSplineDataModelSharedPtr& GetSplineDataModel(const SplineID& id) const noexcept override final;
    virtual void VisitSplineDataModels(const ConstVisitorType<ICurveEditorSplineDataModelSharedPtr>& visitor) const noexcept override final;

    virtual void SetSelectionDataModel(ICurveEditorSelectionDataModelSharedPtr&& selectionDataModel) override final;
    virtual const ICurveEditorSelectionDataModelSharedPtr& GetSelectionDataModel() const noexcept override final;

private:
    SCurveEditorStyle m_EditorStyle;
    std::vector<ICurveEditorSplineDataModelSharedPtr> m_SplinesDataModels;
    ICurveEditorSelectionDataModelSharedPtr m_SelectionDataModel;
};

CCurveEditorDataModel::~CCurveEditorDataModel()
{
    if (m_SelectionDataModel)
    {
        NotifyListeners(&ICurveEditorDataModelListener::OnSelectionDataModelChanged, nullptr);
        m_SelectionDataModel.reset();
    }
}

void CCurveEditorDataModel::SetStyle(SCurveEditorStyle&& style)
{
    m_EditorStyle = std::move(style);
    NotifyListeners(&ICurveEditorDataModelListener::OnStyleChanged, m_EditorStyle);
}

const SCurveEditorStyle& CCurveEditorDataModel::GetStyle() const noexcept
{
    return m_EditorStyle;
}

SplineID CCurveEditorDataModel::GetFreeSplineID() const noexcept
{
    while (true)
    {
        const auto result = ICurveEditorSplineDataModel::GenerateSplineID();
        if (!GetSplineDataModel(result))
            return result;
    }
}

bool CCurveEditorDataModel::AddSplineDataModel(ICurveEditorSplineDataModelUniquePtr&& splineDataModel)
{
    EDITOR_ASSERT(splineDataModel);
    if (!splineDataModel)
        return false;

    const auto collidingSpline = GetSplineDataModel(splineDataModel->GetID());
    EDITOR_ASSERT(!collidingSpline);
    if (collidingSpline)
        return nullptr;

    m_SplinesDataModels.emplace_back(std::move(splineDataModel));
    const auto& addedSplineDataModel = m_SplinesDataModels.back();

    NotifyListeners(&ICurveEditorDataModelListener::OnSplineCreated, addedSplineDataModel);
    return true;
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

void CCurveEditorDataModel::SetSelectionDataModel(ICurveEditorSelectionDataModelSharedPtr&& selectionDataModel)
{
    const auto buffer = std::move(m_SelectionDataModel);
    m_SelectionDataModel = std::move(selectionDataModel);
    NotifyListeners(&ICurveEditorDataModelListener::OnSelectionDataModelChanged, m_SelectionDataModel);
}

const ICurveEditorSelectionDataModelSharedPtr& CCurveEditorDataModel::GetSelectionDataModel() const noexcept
{
    return m_SelectionDataModel;
}

ICurveEditorDataModelUniquePtr ICurveEditorDataModel::Create()
{
    return std::make_unique<CCurveEditorDataModel>();
}