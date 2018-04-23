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
    virtual ~CCurveEditorVisibleToolBase() override = default;

    std::optional<ToolViewHandle> AddToolView(ICurveEditorView& activeEditorView, ToolViewFactory&& viewFactory, EComponentOrder order);
    bool RemoveToolView(const ToolViewHandle& handle);

private:
    struct SToolViewStorage
    {
        SToolViewStorage(ToolViewFactory&& toolFactory, EComponentOrder order) : ViewFactory(std::move(toolFactory)), Order(order) {};
        std::vector<std::pair<ICurveEditorViewComponentWeakPtr, EditorViewComponentHandle>> ViewComponents;

        ToolViewFactory ViewFactory;
        EComponentOrder Order;
    };

    std::map<ToolViewHandle, SToolViewStorage> m_ToolViews;
};

#endif //__CURVE_EDITOR_VISIBLE_TOOL_BASE_H__