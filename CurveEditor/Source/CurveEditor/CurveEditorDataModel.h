#pragma  once
#if !defined(__CURVE_EDITOR_DATA_MODEL_H__)

#include "Style.h"
#include "EditorView.h"
#include "EditorDataModel.h"

class CCurveEditorDataModel final : public IEditorDataModel
{
public:
    CCurveEditorDataModel() = default;
    virtual ~CCurveEditorDataModel() override final = default;

    SCurveEditorStyle& GetStyle() noexcept;
    const SCurveEditorStyle& GetStyle() const noexcept;

    ICurveEditorSplineDataModelSharedPtr AddSplineDataModel(std::string_view name, unsigned int color);
    bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel);

    const std::vector<ICurveEditorSplineDataModelSharedPtr>& GetSplinesDataModels() const noexcept;

private:
    SCurveEditorStyle m_EditorStyle;
    std::vector<ICurveEditorSplineDataModelSharedPtr> m_SplinesDataModels;
};

#endif //__CURVE_EDITOR_DATA_MODEL_H__