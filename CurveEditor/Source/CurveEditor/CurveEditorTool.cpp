#include "pch.h"
#include "CurveEditorTool.h"

class CCurveEditorComponentTool final : public ICurveEditorComponentTool
{
public:
    CCurveEditorComponentTool() = default;
    virtual ~CCurveEditorComponentTool() override final = default;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;

    virtual void OnWheel(const CCurveEditorToolWheelEvent& event) override final;

    virtual void OnClick(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDoubleClick(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnKeyDown(const CCurveEditorToolKeyEvent& event) override final;
    virtual void OnKeyUp(const CCurveEditorToolKeyEvent& event) override final;

    virtual bool AddComponent(ICurveEditorToolUniquePtr&& component) override final;

private:
    template<typename Method, typename... Arguments>
    void NotifyComponents(Method method, Arguments&&... arguments)
    {
        for (const auto& component : m_Components)
        {
            if (component)
                (component.get()->*method)(arguments...);
        }
    }

private:
    std::vector<ICurveEditorToolUniquePtr> m_Components;
};

void CCurveEditorComponentTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnDragBegin, event);
}

void CCurveEditorComponentTool::OnDragUpdate(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnDragUpdate, event);
}

void CCurveEditorComponentTool::OnDragEnd(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnDragEnd, event);
}

void CCurveEditorComponentTool::OnMouseMove(const CCurveEditorToolMouseEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnMouseMove, event);
}

void CCurveEditorComponentTool::OnWheel(const CCurveEditorToolWheelEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnWheel, event);
}

void CCurveEditorComponentTool::OnClick(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnClick, event);
}

void CCurveEditorComponentTool::OnDoubleClick(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnDoubleClick, event);
}

void CCurveEditorComponentTool::OnKeyDown(const CCurveEditorToolKeyEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnKeyDown, event);
}

void CCurveEditorComponentTool::OnKeyUp(const CCurveEditorToolKeyEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnKeyUp, event);
}

bool CCurveEditorComponentTool::AddComponent(ICurveEditorToolUniquePtr&& component)
{
    if (!component)
        return false;

    m_Components.emplace_back(std::move(component));
    return true;
}

ICurveEditorComponentToolUniquePtr ICurveEditorComponentTool::Create()
{
    return std::make_unique<CCurveEditorComponentTool>();
}