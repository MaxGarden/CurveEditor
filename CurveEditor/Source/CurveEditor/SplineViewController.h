#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_CONTROLLER_H__)

#include "EditorController.h"s
#include "CurveEditorViewDataModel.h"

class CCurveEditorViewController final : public IEditorController
{
public:
    CCurveEditorViewController() = default;
    virtual ~CCurveEditorViewController() override final = 0;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept override final;

private:
    void OnDataModelChanged() noexcept;

private:
    CCurveEditorViewDataModelSharedPtr m_DataModel;
};
#endif //__CURVE_EDITOR_VIEW_CONTROLLER_H__