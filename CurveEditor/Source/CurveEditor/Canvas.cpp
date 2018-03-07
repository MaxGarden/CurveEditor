#include "pch.h"
#include "Canvas.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

CWindowCanvas::CWindowCanvas() noexcept :
    m_WindowScreenPosition(0, 0),
    m_WindowScreenSize(0, 0),
    m_ClientOrigin(0, 0),
    m_ClientSize(0, 0),
    m_Zoom(1, 1),
    m_InvertZoom(1, 1)
{
}

CWindowCanvas::CWindowCanvas(const ImVec2& position, const ImVec2& size, const ImVec2& zoom, const ImVec2& origin) noexcept :
    m_WindowScreenPosition(position),
    m_WindowScreenSize(size),
    m_ClientSize(size),
    m_ClientOrigin(origin),
    m_Zoom(zoom),
    m_InvertZoom(1, 1)
{
    EDITOR_ASSERT(m_Zoom.x != 0.0f && m_Zoom.y != 0.0f);

    if (m_Zoom.x == 0.0f)
        m_Zoom.x = 1.0f;

    if (m_Zoom.y == 0.0f)
        m_Zoom.y = 1.0f;

    m_InvertZoom.x = m_Zoom.x ? 1.0f / m_Zoom.x : 1.0f;
    m_InvertZoom.y = m_Zoom.y ? 1.0f / m_Zoom.y : 1.0f;

    EDITOR_ASSERT(m_WindowScreenSize.x > 0 && m_WindowScreenSize.y > 0);
    m_WindowScreenSize.x = std::max(1.0f, m_WindowScreenSize.x);
    m_WindowScreenSize.y = std::max(1.0f, m_WindowScreenSize.y);

    if (m_InvertZoom.x > 1.0f)
        m_ClientSize.x *= m_InvertZoom.x;
    if (m_InvertZoom.y > 1.0f)
        m_ClientSize.y *= m_InvertZoom.y;
}

const ImVec2& CWindowCanvas::GetWindowScreenPosition() const noexcept
{
    return m_WindowScreenPosition;
}

const ImVec2& CWindowCanvas::GetWindowScreenSize() const noexcept
{
    return m_WindowScreenSize;
}

const ImVec2& CWindowCanvas::GetClientOrigin() const noexcept
{
    return m_ClientOrigin;
}

const ImVec2& CWindowCanvas::GetClientSize() const noexcept
{
    return m_ClientSize;
}

const ImVec2& CWindowCanvas::GetZoom() const noexcept
{
    return m_Zoom;
}

const ImVec2& CWindowCanvas::GetInvertZoom() const noexcept
{
    return m_InvertZoom;
}

ax::rectf CWindowCanvas::CalculateVisibleBounds(bool zoom) const noexcept
{
    return ax::rectf(
        to_pointf(FromScreen(m_WindowScreenPosition, zoom)),
        to_pointf(FromScreen(m_WindowScreenPosition + m_WindowScreenSize, zoom)));
}

ImVec2 CWindowCanvas::FromScreen(const ImVec2& point, bool zoom) const noexcept
{
    return ImVec2(
        (point.x - m_WindowScreenPosition.x - m_ClientOrigin.x) * (zoom ? m_InvertZoom.x : 1.0f),
        (point.y - m_WindowScreenPosition.y - m_ClientOrigin.y) * (zoom ? m_InvertZoom.y : 1.0f));
}

ImVec2 CWindowCanvas::ToScreen(const ImVec2& point) const noexcept
{
    return ImVec2(
        (point.x + m_ClientOrigin.x + m_WindowScreenPosition.x),
        (point.y + m_ClientOrigin.y + m_WindowScreenPosition.y));
}

ImVec2 CWindowCanvas::FromClient(const ImVec2& point) const noexcept
{
    return ImVec2(
        (point.x - m_ClientOrigin.x),
        (point.y - m_ClientOrigin.y));
}

ImVec2 CWindowCanvas::ToClient(const ImVec2& point) const noexcept
{
    return ImVec2(
        (point.x + m_ClientOrigin.x),
        (point.y + m_ClientOrigin.y));
}

CEditorCanvas::CEditorCanvas(const ImVec2& unitScaler) :
    m_UnitScaler(unitScaler)
{
    EDITOR_ASSERT(m_UnitScaler.x != 0.0f && m_UnitScaler.y != 0.0f);

    if (m_UnitScaler.x == 0.0f)
        m_UnitScaler.x = 1.0f;

    if (m_UnitScaler.y == 0.0f)
        m_UnitScaler.y = 1.0f;
}

const ImVec2& CEditorCanvas::GetUnitScaler() const noexcept
{
    return m_UnitScaler;
}

void CEditorCanvas::SetUnitScaler(const ImVec2& unitScaler) noexcept
{
    EDITOR_ASSERT(unitScaler.x != 0.0f && unitScaler.y != 0.0f);
    if (unitScaler.x == 0.0f || unitScaler.y == 0.0f)
        return;

    m_UnitScaler = unitScaler;
}

CWindowCanvas& CEditorCanvas::GetWindowCanvas() noexcept
{
    return m_WindowCanvas;
}

const CWindowCanvas& CEditorCanvas::GetWindowCanvas() const noexcept
{
    return m_WindowCanvas;
}

ImVec2 CEditorCanvas::CalculateScaledUnit() const noexcept
{
    const auto& zoom = m_WindowCanvas.GetZoom();

    static const auto calculate = [](const auto& unit, const auto& zoom)
    {
        auto result = unit* zoom;

        while (result > unit * 2.0f)
            result /= 2.0f;

        while (result < unit* 0.5f)
            result *= 2.0f;

        return result;
    };

    return ImVec2(
        calculate(m_UnitScaler.x, zoom.x),
        calculate(m_UnitScaler.y, zoom.y));
}

ImVec2 CEditorCanvas::FromEditor(const ImVec2& value) const noexcept
{
    const auto& zoom = m_WindowCanvas.GetZoom();

    return ImVec2(
        (value.x * m_UnitScaler.x * zoom.x),
        (-value.y * m_UnitScaler.y * zoom.y));
}

ImVec2 CEditorCanvas::ToEditor(const ImVec2& position) const noexcept
{
    const auto& zoom = m_WindowCanvas.GetZoom();

    EDITOR_ASSERT(zoom.x != 0.0f && zoom.y != 0.0f);
    if (zoom.x == 0.0f || zoom.y == 0.0f)
        return ImVec2();

    return ImVec2(
        (position.x / m_UnitScaler.x / zoom.x),
        (-position.y / m_UnitScaler.y / zoom.y));
}