#pragma  once
#if !defined(__EDITOR_H__)

#include "EditorView.h"

class CCurveEditorViewFactory final : public IEditorViewFactory
{
public:
    CCurveEditorViewFactory() = default;
    virtual ~CCurveEditorViewFactory() override final = default;

    virtual std::unique_ptr<IEditorView> Create() override final;
};

#endif //__EDITOR_VIEW_H__