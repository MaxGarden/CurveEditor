#include "pch.h"
#include "SplineDataModel.h"

class CCurveEditorSplineDataModel final : public ICurveEditorSplineDataModel
{
public:
    CCurveEditorSplineDataModel(std::string_view name, unsigned int color);
    virtual ~CCurveEditorSplineDataModel() override final = default;

    virtual std::vector<ax::pointf>& GetControlPoints() noexcept override final;
    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept override final;
    virtual const std::vector<ECurveType>& GetCurvesTypes() const noexcept override final;

    virtual const std::string& GetName() const noexcept override final;
    virtual unsigned int GetColor() const noexcept override final;

private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurvesTypes;
    std::string m_Name;
    unsigned int m_Color = 0;
};

CCurveEditorSplineDataModel::CCurveEditorSplineDataModel(std::string_view name, unsigned int color) :
    m_Name(name),
    m_Color(color)
{
    //for tests
    m_ControlPoints = { { 1.0f, -1.0f }, { 1.01f, -2.0f }, { 1.0f, -5.0f }, { 3.0f, -3.0f },
                        { 3.0f, -7.0f }, { 3.01f, -2.0f }, { 4.0f, -1.0f },
                        { 5.0f, -3.0f }, { 5.01f, -2.0f }, { 6.0f, -1.0f } };
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

unsigned int CCurveEditorSplineDataModel::GetColor() const noexcept
{
    return m_Color;
}

ICurveEditorSplineDataModelUniquePtr ICurveEditorSplineDataModel::Create(std::string_view name, unsigned int color)
{
    return std::make_unique<CCurveEditorSplineDataModel>(name, color);
}