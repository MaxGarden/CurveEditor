#include "pch.h"
#include "SplineDataModel.h"
#include "EditorListenableBase.h"
#include <random>

class CCurveEditorSplineDataModel final : public CEditorListenableBase<ICurveEditorSplineDataModel, ICurveEditorSplineDataModelListener>
{
public:
    CCurveEditorSplineDataModel(SplineID&& id, SplineColor&& color);
    virtual ~CCurveEditorSplineDataModel() override final = default;

    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept override final;
    virtual bool SetControlPoints(const SplineControlPointsPositions& positions) override final;

    virtual const std::vector<ECurveType>& GetCurvesTypes() const noexcept override final;

    virtual const SplineID& GetID() const noexcept override final;
    virtual const SplineColor& GetColor() const noexcept override final;

private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurvesTypes;
    const SplineID m_ID;
    const unsigned int m_Color = 0;
};

CCurveEditorSplineDataModel::CCurveEditorSplineDataModel(SplineID&& id, SplineColor&& color) :
    m_ID(std::move(id)),
    m_Color(std::move(color))
{
    //for tests
    m_ControlPoints = { { 1.0f, -1.0f }, { 1.01f, -2.0f }, { 1.0f, -5.0f }, { 3.0f, -3.0f },
                        { 3.0f, -7.0f }, { 3.01f, -2.0f }, { 4.0f, -1.0f },
                        { 5.0f, -3.0f }, { 5.01f, -2.0f }, { 6.0f, -1.0f } };
}

const std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() const noexcept
{
    return m_ControlPoints;
}

bool CCurveEditorSplineDataModel::SetControlPoints(const SplineControlPointsPositions& positions)
{
    if (positions.empty())
        return false;

    const auto lastControlPointIndex = positions.rbegin()->ControlPointIndex;

    EDITOR_ASSERT(lastControlPointIndex < m_ControlPoints.size());
    if (lastControlPointIndex >= m_ControlPoints.size())
        return false;

    VisitObjectsContainer(positions, [this](const auto& singlePosition)
    {
        m_ControlPoints[singlePosition.ControlPointIndex] = singlePosition.Position;
    });

    NotifyListeners(&ICurveEditorSplineDataModelListener::OnControlPointsPositionsChanged, positions);
    return true;
}

const std::vector<ECurveType>& CCurveEditorSplineDataModel::GetCurvesTypes() const noexcept
{
    return m_CurvesTypes;
}

const SplineID& CCurveEditorSplineDataModel::GetID() const noexcept
{
    return m_ID;
}

const SplineColor& CCurveEditorSplineDataModel::GetColor() const noexcept
{
    return m_Color;
}

ICurveEditorSplineDataModelUniquePtr ICurveEditorSplineDataModel::Create(SplineID id, SplineColor color)
{
    return std::make_unique<CCurveEditorSplineDataModel>(std::move(id), std::move(color));
}

SplineID ICurveEditorSplineDataModel::GenerateSplineID() noexcept
{
    static std::mt19937_64 generator{ std::random_device{}() };
    return std::uniform_int_distribution<SplineID>{1, UINT64_MAX}(generator);
}

const SplineID& ICurveEditorSplineDataModel::InvalidSplineID() noexcept
{
    static SplineID invalidID = 0;
    return invalidID;
}

SSplineControlPointSinglePosition::SSplineControlPointSinglePosition(size_t controlPointIndex, ax::pointf position) :
    ControlPointIndex(controlPointIndex),
    Position(position)
{
}

bool SSplineControlPointSinglePosition::operator<(const SSplineControlPointSinglePosition& rhs) const noexcept
{
    return ControlPointIndex < rhs.ControlPointIndex;
}