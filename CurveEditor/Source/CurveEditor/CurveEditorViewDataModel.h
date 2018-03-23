#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_DATA_MODEL_H__)

#include "Canvas.h"
#include "EditorDataModel.h"

class ICurveEditorViewDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorViewDataModel() override = default;

    virtual CEditorCanvas& GetEditorCanvas() noexcept = 0;
    virtual const CEditorCanvas& GetEditorCanvas() const noexcept = 0;

    static ICurveEditorViewDataModelUniquePtr Create();
};

#endif //__CURVE_EDITOR_VIEW_DATA_MODEL_H__