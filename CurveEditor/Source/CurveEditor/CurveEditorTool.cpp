#include "pch.h"
#include "CurveEditorTool.h"

class CCurveEditorComponentTool final : public ICurveEditorComponentTool
{
public:
    CCurveEditorComponentTool() = default;
    virtual ~CCurveEditorComponentTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override final;

    virtual void OnClickDown(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) override final;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) override final;

    virtual bool AddComponent(ICurveEditorToolUniquePtr&& component) override final;

private:
    template<typename Method, typename... Arguments>
    void NotifyComponents(Method method, Arguments&&... arguments)
    {
        for (const auto& component : m_Components)
        {
            if (component)
                (component.get()->*method)(std::forward<Arguments>(arguments)...);
        }
    }

private:
    std::vector<ICurveEditorToolUniquePtr> m_Components;
};

void CCurveEditorComponentTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnAcquired, event);
}

void CCurveEditorComponentTool::OnReleased(const CCurveEditorToolEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnReleased, event);
}

void CCurveEditorComponentTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnDragBegin, event);
}

void CCurveEditorComponentTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
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

void CCurveEditorComponentTool::OnWheel(const CCurveEditorToolMouseWheelEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnWheel, event);
}

void CCurveEditorComponentTool::OnClickDown(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnClickDown, event);
}

void CCurveEditorComponentTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnClickUp, event);
}

void CCurveEditorComponentTool::OnModifierActivated(const CCurveEditorToolModifierEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnModifierActivated, event);
}

void CCurveEditorComponentTool::OnModifierDeactivated(const CCurveEditorToolModifierEvent& event)
{
    NotifyComponents(&ICurveEditorTool::OnModifierDeactivated, event);
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