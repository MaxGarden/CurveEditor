#pragma once
#if !defined(__CURVE_EDITOR_DATA_MODEL_SERIALIZER_H__)

#include "EditorSerializer.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorSplineDataModelSerializer.h"
#include "CurveEditorSelectionDataModelSerializer.h"

class ICurveEditorDataModelSerializer : public IEditorSerializer
{
public:
    virtual ~ICurveEditorDataModelSerializer() override = default;

    static ICurveEditorDataModelSerializerUniquePtr Create();
};

class CCurveEditorSerializable final : public xmls::Serializable
{
public:
    CCurveEditorSerializable();
    virtual ~CCurveEditorSerializable() override final = default;

    bool Set(const ICurveEditorDataModel& curveEditorDataModel);
    ICurveEditorDataModelUniquePtr CreateCurveEditorDataModel() const;

private:
    bool SetSplines(const ICurveEditorDataModel& curveEditorDataModel);
    bool SetSelection(const ICurveEditorDataModel& curveEditorDataModel);

    bool GetSplines(ICurveEditorDataModel& curveEditorDataModel) const;
    bool GetSelection(ICurveEditorDataModel& curveEditorDataModel) const;

private:
    xmls::Collection<CCurveEditorSplineSerializable> m_Splines;
    CCurveEditorSelectionSerializable m_Selection;
};

#endif //__CURVE_EDITOR_DATA_MODEL_SERIALIZER_H__