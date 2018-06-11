#pragma once
#if !defined(__CURVE_EDITOR_SELECTION_DATA_MODEL_SERIALIZER_H__)

#include "EditorSerializer.h"
#include "CurveEditorSerializerTypes.h"
#include "CurveEditorSelectionDataModel.h"

class ICurveEditorSelectionDataModelSerializer : public IEditorSerializer
{
public:
    virtual ~ICurveEditorSelectionDataModelSerializer() override = default;

    static ICurveEditorSelectionDataModelSerializerUniquePtr Create();
};

class CCurveEditorSingleSelectionSerializable final : public xmls::Serializable
{
public:
    CCurveEditorSingleSelectionSerializable();
    CCurveEditorSingleSelectionSerializable(const CurveEditorDataModelSingleSelection& singleSelection);
    virtual ~CCurveEditorSingleSelectionSerializable() override final = default;

    CurveEditorDataModelSingleSelection GetSingleSelection() const noexcept;

private:
    void Initialize();

private:
    xmls::xSizeT m_SplineID;
    xmls::xSizeT m_ControlPointIndex;
};

class CCurveEditorSelectionSerializable final : public xmls::Serializable
{
public:
    CCurveEditorSelectionSerializable();
    virtual ~CCurveEditorSelectionSerializable() override final = default;

    bool Set(const ICurveEditorSelectionDataModel& selectionDataModel);
    ICurveEditorSelectionDataModelUniquePtr CreateSelectionDataModel() const;

private:
    xmls::xSizeT m_SelectionMode;
    xmls::Collection<CCurveEditorSingleSelectionSerializable> m_Selection;
};

#endif //__CURVE_EDITOR_SELECTION_DATA_MODEL_SERIALIZER_H__