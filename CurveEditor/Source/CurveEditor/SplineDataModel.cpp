#include "pch.h"
#include "SplineDataModel.h"

class CCurveEditorSplineDataModel final : public ICurveEditorSplineDataModel
{
public:
    CCurveEditorSplineDataModel(std::string&& name);
    virtual ~CCurveEditorSplineDataModel() override final = default;

    virtual std::vector<ax::pointf>& GetControlPoints() noexcept override final;
    const std::vector<ax::pointf>& GetControlPoints() const noexcept override final;
    const std::vector<ECurveType>& GetCurvesTypes() const noexcept override final;

    const std::string& GetName() const noexcept override final;

private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurvesTypes;
    std::string m_Name;
};

CCurveEditorSplineDataModel::CCurveEditorSplineDataModel(std::string&& name) :
    m_Name(std::move(name))
{
    //for tests
    m_ControlPoints = { { 0.0f, 0.0f }, { 3.0f, 200.0f }, { 20.0f, 20.0f }, { 400.0f, 50.0f } };
}

std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() noexcept
{
    return m_ControlPoints;
}

const std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() const noexcept
{
    return m_ControlPoints;
}

const std::vector<ECurveType>& CCurveEditorSplineDataModel::GetCurvesTypes() const noexcept
{
    return m_CurvesTypes;
}

const std::string& CCurveEditorSplineDataModel::GetName() const noexcept
{
    return m_Name;
}

ICurveEditorSplineDataModelUniquePtr ICurveEditorSplineDataModel::Create(std::string&& name)
{
    return std::make_unique<CCurveEditorSplineDataModel>(std::move(name));
}