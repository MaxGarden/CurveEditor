#include "pch.h"
#include "CurveEditorSelectionDataModelSerializer.h"

class CCurveEditorSelectionDataModelSerializer final : public ICurveEditorSelectionDataModelSerializer
{
public:
    CCurveEditorSelectionDataModelSerializer() = default;
    virtual ~CCurveEditorSelectionDataModelSerializer() override final = default;

    virtual bool Serialize(IEditorDataModel& dataModel, SerializedData& serializedData) override final;
    virtual IEditorDataModelUniquePtr Load(const SerializedData& serializedData) override final;
};

bool CCurveEditorSelectionDataModelSerializer::Serialize(IEditorDataModel& dataModel, SerializedData& serializedData)
{
    const auto selectionDataModel = dynamic_cast<ICurveEditorSelectionDataModel*>(&dataModel);
    EDITOR_ASSERT(selectionDataModel);
    if (!selectionDataModel)
        return false;

    CCurveEditorSelectionSerializable selectionSerializable;

    const auto result = selectionSerializable.Set(*selectionDataModel);
    EDITOR_ASSERT(result);
    if (!result)
        return false;

    serializedData = selectionSerializable.toXML();
    return true;
}

IEditorDataModelUniquePtr CCurveEditorSelectionDataModelSerializer::Load(const SerializedData& serializedData)
{
    CCurveEditorSelectionSerializable selectionSerializable;

    const auto loadResult = xmls::Serializable::fromXML(serializedData, &selectionSerializable);
    EDITOR_ASSERT(loadResult);
    if (!loadResult)
        return nullptr;

    return selectionSerializable.CreateSelectionDataModel();
}

ICurveEditorSelectionDataModelSerializerUniquePtr ICurveEditorSelectionDataModelSerializer::Create()
{
    return std::make_unique<CCurveEditorSelectionDataModelSerializer>();
}

CCurveEditorSingleSelectionSerializable::CCurveEditorSingleSelectionSerializable()
{
    Initialize();
}

CCurveEditorSingleSelectionSerializable::CCurveEditorSingleSelectionSerializable(const CurveEditorDataModelSingleSelection& singleSelection)
{
    Initialize();
    m_SplineID = singleSelection.first;
    m_ControlPointIndex = singleSelection.second;
}

CurveEditorDataModelSingleSelection CCurveEditorSingleSelectionSerializable::GetSingleSelection() const noexcept
{
    return { m_SplineID.value(), m_ControlPointIndex.value() };
}

void CCurveEditorSingleSelectionSerializable::Initialize()
{
    setClassName("SingleSelection");
    Register("SplineID", &m_SplineID);
    Register("ControlPointIndex", &m_ControlPointIndex);
}

CCurveEditorSelectionSerializable::CCurveEditorSelectionSerializable()
{
    setClassName("Selection");
    Register("SelectionMode", &m_SelectionMode);
    Register("SelectionData", &m_Selection);
}

bool CCurveEditorSelectionSerializable::Set(const ICurveEditorSelectionDataModel& selectionDataModel)
{
    m_SelectionMode = static_cast<size_t>(selectionDataModel.GetSelectionMode());

    const auto& selection = selectionDataModel.GetSelection();

    m_Selection.Clear();
    for (const auto& singleSelection : selection)
        m_Selection.addItem(new CCurveEditorSingleSelectionSerializable{ singleSelection });

    return true;
}

ICurveEditorSelectionDataModelUniquePtr CCurveEditorSelectionSerializable::CreateSelectionDataModel() const
{
    auto selectionDataModel = ICurveEditorSelectionDataModel::Create();
    EDITOR_ASSERT(selectionDataModel);
    if (!selectionDataModel)
        return nullptr;

    const auto selectionMode = static_cast<ECurveEditorSplineComponentType>(m_SelectionMode.value());
    selectionDataModel->SetSelectionMode(selectionMode);

    CurveEditorDataModelSelection selection;

    const auto selectionSize = m_Selection.size();
    for (auto i = 0u; i < selectionSize; ++i)
    {
        const auto singleSelection = m_Selection.getItem(i);
        EDITOR_ASSERT(singleSelection);
        if (!singleSelection)
            continue;

        selection.emplace(singleSelection->GetSingleSelection());
    }

    if (!selection.empty())
    {
        const auto result = selectionDataModel->AddToSelection(selection);
        EDITOR_ASSERT(result);
    }

    return selectionDataModel;
}