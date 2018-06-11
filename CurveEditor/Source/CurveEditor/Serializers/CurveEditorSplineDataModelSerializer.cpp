#include "pch.h"
#include "CurveEditorSplineDataModelSerializer.h"

class CCurveEditorSplineDataModelSerializer final : public ICurveEditorSplineDataModelSerializer
{
public:
    CCurveEditorSplineDataModelSerializer() = default;
    virtual ~CCurveEditorSplineDataModelSerializer() override final = default;

    virtual bool Serialize(IEditorDataModel& dataModel, SerializedData& serializedData) override final;
    virtual IEditorDataModelUniquePtr Load(const SerializedData& serializedData) override final;
};

CCurveEditorSplineSerializable::CCurveEditorSplineSerializable()
{
    setClassName("Spline");
    Register("ID", &m_ID);
    Register("Type", &m_Type);
    Register("Color", &m_Color);
    Register("ControlPoints", &m_ControlPoints);
}

bool CCurveEditorSplineSerializable::Set(const ICurveEditorSplineDataModel& selectionDataModel)
{
    m_ID = static_cast<size_t>(selectionDataModel.GetID());
    m_Type = static_cast<size_t>(selectionDataModel.GetType());
    m_Color = static_cast<int>(selectionDataModel.GetColor());

    const auto& controlPoints = selectionDataModel.GetControlPoints();

    m_ControlPoints.Clear();
    for (const auto& controlPoint : controlPoints)
        m_ControlPoints.addItem(new CCurveEditorPointfSeriaizable{ controlPoint });

    return true;
}

ICurveEditorSplineDataModelUniquePtr CCurveEditorSplineSerializable::CreateSplineDataModel() const
{
    const auto id = static_cast<SplineID>(m_ID.value());
    const auto color = static_cast<SplineColor>(m_Color.value());
    const auto type = static_cast<ECurveEditorSplineType>(m_Type.value());

    auto result = ICurveEditorSplineDataModel::Create(id, color, type);
    EDITOR_ASSERT(result);
    if (!result)
        return nullptr;

    SplineControlPointsPositions controlPoints;

    const auto controlPointsSize = m_ControlPoints.size();
    for (auto i = 0u; i < controlPointsSize; ++i)
    {
        const auto controlPoint = m_ControlPoints.getItem(i);
        EDITOR_ASSERT(controlPoint);
        if (!controlPoint)
            continue;

        controlPoints.emplace(i, controlPoint->GetValue());
    }

    const auto addResult = result->AddControlPoints(controlPoints);
    EDITOR_ASSERT(addResult);
    if (!addResult)
        return nullptr;

    return result;
}

bool CCurveEditorSplineDataModelSerializer::Serialize(IEditorDataModel& dataModel, SerializedData& serializedData)
{
    const auto selectionDataModel = dynamic_cast<ICurveEditorSplineDataModel*>(&dataModel);
    EDITOR_ASSERT(selectionDataModel);
    if (!selectionDataModel)
        return false;

    CCurveEditorSplineSerializable splineSerializable;

    const auto result = splineSerializable.Set(*selectionDataModel);
    EDITOR_ASSERT(result);
    if (!result)
        return false;

    serializedData = splineSerializable.toXML();
    return true;
}

IEditorDataModelUniquePtr CCurveEditorSplineDataModelSerializer::Load(const SerializedData& serializedData)
{
    CCurveEditorSplineSerializable splineSerializable;

    const auto loadResult = xmls::Serializable::fromXML(serializedData, &splineSerializable);
    EDITOR_ASSERT(loadResult);
    if (!loadResult)
        return nullptr;

    return splineSerializable.CreateSplineDataModel();
}

ICurveEditorSplineDataModelSerializerUniquePtr ICurveEditorSplineDataModelSerializer::Create()
{
    return std::make_unique<CCurveEditorSplineDataModelSerializer>();
}