#pragma  once
#if !defined(__CURVE_EDITOR_DATA_MODEL_H__)

#include "EditorView.h"
#include "EditorDataModel.h"
#include "Style.h"

class CCurveEditorDataModel final : public IEditorDataModel
{
public:
    CCurveEditorDataModel() = default;
    virtual ~CCurveEditorDataModel() override final = default;

    SEditorStyle& GetStyle() noexcept;
    const SEditorStyle& GetStyle() const noexcept;

private:
    SEditorStyle m_EditorStyle;
};


#endif //__CURVE_EDITOR_DATA_MODEL_H__