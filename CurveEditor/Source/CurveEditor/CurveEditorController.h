#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorController.h"

class CCurveEditorController final : public IEditorController
{
public:
    CCurveEditorController() = default;
    virtual ~CCurveEditorController() override final = default;
};

#endif //__CURVE_EDITOR_CONTROLLER_H__