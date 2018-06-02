#pragma once
#if !defined(__CURVE_EDITOR_VIEW_H__)

#include "Canvas.h"
#include "CurveEditorController.h"
#include "EditorViewBase.h"

using EditorViewComponentHandle = size_t;

enum class EComponentOrder
{
    ToolHandler,
    Background,
    Splines,
    Foreground,
    Selection,
    Helper
};

class ICurveEditorViewComponent : public IEditorView
{
public:
    virtual ~ICurveEditorViewComponent() override = default;

    virtual bool Initialize() = 0;
    virtual ICurveEditorView& GetEditorView() const noexcept = 0;
};

class ICurveEditorView : public IEditorView
{
public:
    virtual ~ICurveEditorView() override = default;

    virtual bool Initialize() = 0;

    virtual CEditorCanvas& GetCanvas() noexcept = 0;
    virtual const CEditorCanvas& GetCanvas() const noexcept = 0;

    virtual std::optional<EditorViewComponentHandle> AddViewComponent(ICurveEditorViewComponentUniquePtr&& viewComponent, EComponentOrder order) = 0;
    virtual bool RemoveViewComponent(const EditorViewComponentHandle& handle) = 0;

    virtual void VisitViewComponents(const ConstInterruptibleVisitorType <ICurveEditorViewComponentSharedPtr>& visitor) const noexcept = 0;
    virtual ICurveEditorViewComponentSharedPtr GetViewComponent(const EditorViewComponentHandle& handle) const noexcept = 0;

    virtual const SCurveEditorStyle& GetEditorStyle() const noexcept = 0;

    static ICurveEditorViewUniquePtr Create();
};

#endif //__CURVE_EDITOR_VIEW_H__