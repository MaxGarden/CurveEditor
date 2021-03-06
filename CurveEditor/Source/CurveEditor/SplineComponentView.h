#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_COMPONENT_VIEW_H__)

#include <ax/ax.h>
#include "Style.h"
#include "EditorView.h"
#include "SplineDataModel.h"
#include "EditorRenderable.h"
#include "SplineController.h"

class ICurveEditorSplineComponentView : public IEditorView
{
public:
    ICurveEditorSplineComponentView() = default;
    virtual ~ICurveEditorSplineComponentView() override = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept = 0;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept = 0;

    virtual ECurveEditorSplineComponentType GetType() const noexcept = 0;

    virtual const SplineID& GetSplineID() const noexcept = 0;
    virtual std::optional<size_t> GetIndex() const noexcept = 0;

    virtual bool SetPosition(const ax::pointf& position) = 0;
    virtual std::optional<ax::pointf> GetPosition() const noexcept = 0;

    virtual std::optional<ax::rectf> CalculateBounds(bool screenTranslations) const noexcept = 0;

    virtual IEditorRenderableUniquePtr CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const = 0;
};

#endif //__CURVE_EDITOR_SPLINE_COMPONENT_VIEW_H__