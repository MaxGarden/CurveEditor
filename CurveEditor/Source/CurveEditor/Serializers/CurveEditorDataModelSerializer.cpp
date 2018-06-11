#include "pch.h"
#include "CurveEditorDataModelSerializer.h"
#include "CurveEditorSelectionDataModelSerializer.h"
#include "CurveEditorSelectionDataModel.h"
#include "CurveEditorSplineDataModelSerializer.h"
#include "SplineDataModel.h"

class CCurveEditorDataModelSerializer final : public ICurveEditorDataModelSerializer
{
public:
    CCurveEditorDataModelSerializer() = default;
    virtual ~CCurveEditorDataModelSerializer() override final = default;

    virtual bool Serialize(IEditorDataModel& dataModel, SerializedData& serializedData) override final;
    virtual IEditorDataModelUniquePtr Load(const SerializedData& sertializedData) override final;
};

bool CCurveEditorDataModelSerializer::Serialize(IEditorDataModel& dataModel, SerializedData& serializedData)
{
    const auto curveEditorDataModel = dynamic_cast<ICurveEditorDataModel*>(&dataModel);
    EDITOR_ASSERT(curveEditorDataModel);
    if (!curveEditorDataModel)
        return false;

    CCurveEditorSerializable curveEditorSerializable;

    const auto result = curveEditorSerializable.Set(*curveEditorDataModel);
    EDITOR_ASSERT(result);
    if (!result)
        return false;

    serializedData = curveEditorSerializable.toXML();
    return true;
}

IEditorDataModelUniquePtr CCurveEditorDataModelSerializer::Load(const SerializedData& serializedData)
{
    CCurveEditorSerializable curveEditorSerializable;

    const auto loadResult = xmls::Serializable::fromXML(serializedData, &curveEditorSerializable);
    EDITOR_ASSERT(loadResult);
    if (!loadResult)
        return nullptr;

    return curveEditorSerializable.CreateCurveEditorDataModel();
}

ICurveEditorDataModelSerializerUniquePtr ICurveEditorDataModelSerializer::Create()
{
    return std::make_unique<CCurveEditorDataModelSerializer>();
}

CCurveEditorSerializable::CCurveEditorSerializable()
{
    setClassName("CurveEditor");
    Register("Splines", &m_Splines);
    Register("Selection", &m_Selection);
}

bool CCurveEditorSerializable::Set(const ICurveEditorDataModel& curveEditorDataModel)
{
    auto result = true;

    result &= SetSplines(curveEditorDataModel);
    result &= SetSelection(curveEditorDataModel);

    EDITOR_ASSERT(result);
    return result;
}

ICurveEditorDataModelUniquePtr CCurveEditorSerializable::CreateCurveEditorDataModel() const
{
    auto curveEditorDataModel = ICurveEditorDataModel::Create();
    EDITOR_ASSERT(curveEditorDataModel);
    if (!curveEditorDataModel)
        return nullptr;

    auto result = true;

    result &= GetSplines(*curveEditorDataModel);
    result &= GetSelection(*curveEditorDataModel);

    EDITOR_ASSERT(result);
    if (!result)
        return nullptr;

    return curveEditorDataModel;
}

bool CCurveEditorSerializable::SetSplines(const ICurveEditorDataModel& curveEditorDataModel)
{
    auto splineSerializationResult = true;

    m_Splines.Clear();
    curveEditorDataModel.VisitSplineDataModels([this, &splineSerializationResult](const auto& splineDataModel)
    {
        splineSerializationResult &= (splineDataModel != nullptr);
        if (!splineSerializationResult)
            return;

        auto splineSerializble = std::make_unique<CCurveEditorSplineSerializable>();

        splineSerializationResult &= splineSerializble->Set(*splineDataModel);
        EDITOR_ASSERT(splineSerializationResult);
        if (splineSerializationResult)
            m_Splines.addItem(splineSerializble.release());
    });

    return splineSerializationResult;
}

bool CCurveEditorSerializable::SetSelection(const ICurveEditorDataModel& curveEditorDataModel)
{
    const auto& selectionDataModel = curveEditorDataModel.GetSelectionDataModel();
    EDITOR_ASSERT(selectionDataModel);
    if (!selectionDataModel)
        return false;

    return m_Selection.Set(*selectionDataModel);
}

bool CCurveEditorSerializable::GetSplines(ICurveEditorDataModel& curveEditorDataModel) const
{
    const auto splinesSize = m_Splines.size();
    for (auto i = 0u; i < splinesSize; ++i)
    {
        const auto spline = m_Splines.getItem(i);
        EDITOR_ASSERT(spline);
        if (!spline)
            return false;

        const auto result = curveEditorDataModel.AddSplineDataModel(spline->CreateSplineDataModel());
        EDITOR_ASSERT(result);
        if (!result)
            return false;
    }

    return true;
}

bool CCurveEditorSerializable::GetSelection(ICurveEditorDataModel& curveEditorDataModel) const
{
    auto selectionDataModel = m_Selection.CreateSelectionDataModel();
    EDITOR_ASSERT(selectionDataModel);
    if (!selectionDataModel)
        return false;

    curveEditorDataModel.SetSelectionDataModel(std::move(selectionDataModel));
    return true;
}