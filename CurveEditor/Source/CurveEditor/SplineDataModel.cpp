#include "pch.h"
#include "SplineDataModel.h"

class CCurveEditorSplineDataModel final : public ICurveEditorSplineDataModel
{
public:
    CCurveEditorSplineDataModel(std::string&& name);
    virtual ~CCurveEditorSplineDataModel() override final = default;

    std::vector<ax::pointf>& GetControlPoints() noexcept override final;
    const std::string& GetName() const noexcept override final;

private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurveTypes;
    std::string m_Name;
};

CCurveEditorSplineDataModel::CCurveEditorSplineDataModel(std::string&& name) :
    m_Name(std::move(name))
{
}

std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() noexcept
{
    return m_ControlPoints;
}

const std::string& CCurveEditorSplineDataModel::GetName() const noexcept
{
    return m_Name;
}

ICurveEditorSplineDataModelUniquePtr ICurveEditorSplineDataModel::Create(std::string&& name)
{
    return std::make_unique<CCurveEditorSplineDataModel>(std::move(name));
}