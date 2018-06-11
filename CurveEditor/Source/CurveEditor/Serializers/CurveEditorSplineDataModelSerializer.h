#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_DATA_MODEL_SERIALIZER_H__)

#include "EditorSerializer.h"
#include "SplineDataModel.h"
#include "CurveEditorSerializerTypes.h"

class ICurveEditorSplineDataModelSerializer : public IEditorSerializer
{
public:
    virtual ~ICurveEditorSplineDataModelSerializer() override = default;

    static ICurveEditorSplineDataModelSerializerUniquePtr Create();
};

class CCurveEditorSplineSerializable final : public xmls::Serializable
{
public:
    CCurveEditorSplineSerializable();
    virtual ~CCurveEditorSplineSerializable() override final = default;

    bool Set(const ICurveEditorSplineDataModel& splineDataModel);
    ICurveEditorSplineDataModelUniquePtr CreateSplineDataModel() const;

private:
    xmls::xSizeT m_ID;
    xmls::xSizeT m_Type;
    xmls::xInt m_Color;
    xmls::Collection<CCurveEditorPointfSeriaizable> m_ControlPoints;
};

#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_SERIALIZER_H__