#pragma  once
#if !defined(__CURVE_EDITOR_FACTORY_H__)

#include "CurveEditorView.h"

class CCurveEditorViewFactory final : public IEditorViewFactory
{
public:
    CCurveEditorViewFactory() = default;
    virtual ~CCurveEditorViewFactory() override final = default;

    virtual std::unique_ptr<IEditorView> Create() override final;
};

#endif //__CURVE_EDITOR_FACTORY_H__