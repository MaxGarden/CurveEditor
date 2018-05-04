#pragma  once
#if !defined(__CURVE_EDITOR_VISIBLE_TOOL_BASE_H__)

#include "CurveEditorView.h"
#include "CurveEditorToolBase.h"

using ToolViewHandle = size_t;
using ToolViewFactory = std::function<ICurveEditorViewComponentUniquePtr(ICurveEditorView&)>;

class CCurveEditorVisibleToolBase : public CCurveEditorToolBase
{
public:
    CCurveEditorVisibleToolBase() = default;
    virtual ~CCurveEditorVisibleToolBase() override;

    std::optional<ToolViewHandle> AddToolView(ICurveEditorView& activeEditorView, ToolViewFactory&& viewFactory, EComponentOrder order);
    std::optional<ToolViewHandle> AddToolView(ICurveEditorView& activeEditorView, ICurveEditorViewComponentUniquePtr&& viewComponent, EComponentOrder order);

    bool RemoveToolView(const ToolViewHandle& handle);

private:
    struct SToolViewStorage
    {
        SToolViewStorage(ToolViewFactory&& toolFactory, EComponentOrder order, bool allowMultiView) : ViewFactory(std::move(toolFactory)), Order(order), AllowMultiView(allowMultiView) {};
        std::vector<std::pair<ICurveEditorViewComponentWeakPtr, EditorViewComponentHandle>> ViewComponents;

        ToolViewFactory ViewFactory;
        EComponentOrder Order;
        bool AllowMultiView;
    };

    std::map<ToolViewHandle, SToolViewStorage> m_ToolViews;
};

#endif //__CURVE_EDITOR_VISIBLE_TOOL_BASE_H__