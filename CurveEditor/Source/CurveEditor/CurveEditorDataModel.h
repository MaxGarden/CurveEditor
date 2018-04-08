#pragma  once
#if !defined(__CURVE_EDITOR_DATA_MODEL_H__)

#include "Style.h"
#include "EditorView.h"
#include "EditorDataModel.h"
#include "EditorListenableBase.h"

class ICurveEditorDataModelListener : public IEditorListener
{
public:
    virtual ~ICurveEditorDataModelListener() override = default;

    virtual void OnStyleChanged(const SCurveEditorStyle& style) = 0;

    virtual void OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) = 0;
    virtual void OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) = 0;
};

class ICurveEditorDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorDataModel() override = default;

    virtual void SetStyle(SCurveEditorStyle&& style) = 0;
    virtual const SCurveEditorStyle& GetStyle() const noexcept = 0;

    virtual ICurveEditorSplineDataModelSharedPtr AddSplineDataModel(std::string_view name, unsigned int color) = 0;
    virtual bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) = 0;

    virtual const std::vector<ICurveEditorSplineDataModelSharedPtr>& GetSplinesDataModels() const noexcept = 0;

    static ICurveEditorDataModelUniquePtr Create();
};

#endif //__CURVE_EDITOR_DATA_MODEL_H__