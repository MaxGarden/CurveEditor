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

    SEditorStyle& GetStyle() noexcept;
    const SEditorStyle& GetStyle() const noexcept;

    ICurveEditorSplineDataModelSharedPtr AddSplineDataModel(std::string&& name);
    bool RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel);

    const std::vector<ICurveEditorSplineDataModelSharedPtr>& GetSplinesDataModels() const noexcept;

private:
    SEditorStyle m_EditorStyle;
    std::vector<ICurveEditorSplineDataModelSharedPtr> m_SplinesDataModels;
};

#endif //__CURVE_EDITOR_DATA_MODEL_H__