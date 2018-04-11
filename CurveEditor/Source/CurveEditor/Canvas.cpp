#include "pch.h"
#include "Canvas.h"

CWindowCanvas::CWindowCanvas() noexcept :
    m_WindowScreenPosition(0, 0),
    m_WindowScreenSize(0, 0),
    m_ClientOrigin(0, 0),
    m_ClientSize(0, 0),
    m_Zoom(1, 1),
    m_InvertZoom(1, 1)
{
}

CWindowCanvas::CWindowCanvas(const ax::pointf& position, const ax::sizef& size, const ax::pointf& zoom, const ax::pointf& origin) noexcept :
    m_WindowScreenPosition(position),
    m_InvertZoom(1, 1)
{
    SetZoom(zoom);
    SetWindowScreenSize(size);
    SetClientOrigin(origin);
}

const ax::pointf& CWindowCanvas::GetWindowScreenPosition() const noexcept
{
    return m_WindowScreenPosition;
}

void CWindowCanvas::SetWindowScreenPosition(const ax::pointf& position) noexcept
{
    m_WindowScreenPosition = position;
}

const ax::sizef& CWindowCanvas::GetWindowScreenSize() const noexcept
{
    return m_WindowScreenSize;
}

void CWindowCanvas::SetWindowScreenSize(const ax::sizef& size) noexcept
{
    if (m_WindowScreenSize == size)
        return;

    m_WindowScreenSize = m_ClientSize = size;

    EDITOR_ASSERT(m_WindowScreenSize.w > 0 && m_WindowScreenSize.h > 0);
    m_WindowScreenSize.w = std::max(1.0f, m_WindowScreenSize.w);
    m_WindowScreenSize.h = std::max(1.0f, m_WindowScreenSize.h);

    RefreshClientSize();
}

const ax::pointf& CWindowCanvas::GetClientOrigin() const noexcept
{
    return m_ClientOrigin;
}

void CWindowCanvas::SetClientOrigin(const ax::pointf& origin) noexcept
{
    m_ClientOrigin = origin;
}

const ax::sizef& CWindowCanvas::GetClientSize() const noexcept
{
    return m_ClientSize;
}

const ax::pointf& CWindowCanvas::GetZoom() const noexcept
{
    return m_Zoom;
}

void CWindowCanvas::SetZoom(const ax::pointf& zoom) noexcept
{
    if (m_Zoom == zoom)
        return;

    m_Zoom = zoom;
    EDITOR_ASSERT(m_Zoom.x != 0.0f && m_Zoom.y != 0.0f);

    if (m_Zoom.x == 0.0f)
        m_Zoom.x = 1.0f;

    if (m_Zoom.y == 0.0f)
        m_Zoom.y = 1.0f;

    m_InvertZoom.x = m_Zoom.x ? 1.0f / m_Zoom.x : 1.0f;
    m_InvertZoom.y = m_Zoom.y ? 1.0f / m_Zoom.y : 1.0f;

    RefreshClientSize();
}

void CWindowCanvas::RefreshClientSize() noexcept
{
    if (m_InvertZoom.x > 1.0f)
        m_ClientSize.w *= m_InvertZoom.x;
    if (m_InvertZoom.y > 1.0f)
        m_ClientSize.h *= m_InvertZoom.y;
}

const ax::pointf& CWindowCanvas::GetInvertZoom() const noexcept
{
    return m_InvertZoom;
}

ax::rectf CWindowCanvas::CalculateVisibleBounds() const noexcept
{
    return ax::rectf{
        FromScreen(m_WindowScreenPosition),
        FromScreen(m_WindowScreenPosition + m_WindowScreenSize) };
}

ax::pointf CWindowCanvas::FromScreen(const ax::pointf& point) const noexcept
{
    EDITOR_ASSERT((m_InvertZoom.x != 0.0f && m_InvertZoom.y != 0.0f));
    if (m_InvertZoom.x == 0.0f || m_InvertZoom.y == 0.0f)
        return {};

    return ax::pointf{
        (point.x - m_WindowScreenPosition.x - m_ClientOrigin.x) * m_InvertZoom.x,
        (point.y - m_WindowScreenPosition.y - m_ClientOrigin.y) * m_InvertZoom.y };
}

ax::pointf CWindowCanvas::ToScreen(const ax::pointf& point) const noexcept
{
    EDITOR_ASSERT(m_Zoom.x != 0.0f && m_Zoom.y != 0.0f);
    if ((m_Zoom.x == 0.0f || m_Zoom.y == 0.0f))
        return {};

    return ax::pointf{
        (point.x *  m_Zoom.x + m_ClientOrigin.x + m_WindowScreenPosition.x) ,
        (point.y *  m_Zoom.y + m_ClientOrigin.y + m_WindowScreenPosition.y) , };
}

ax::pointf CWindowCanvas::FromClient(const ax::pointf& point) const noexcept
{
    return ax::pointf{
        (point.x - m_ClientOrigin.x),
        (point.y - m_ClientOrigin.y)};
}

ax::pointf CWindowCanvas::ToClient(const ax::pointf& point) const noexcept
{
    return ax::pointf{
        (point.x + m_ClientOrigin.x),
        (point.y + m_ClientOrigin.y) };
}

CEditorCanvas::CEditorCanvas(const ax::pointf& unitScaler) :
    m_UnitScaler(unitScaler)
{
    EDITOR_ASSERT(m_UnitScaler.x != 0.0f && m_UnitScaler.y != 0.0f);

    if (m_UnitScaler.x == 0.0f)
        m_UnitScaler.x = 1.0f;

    if (m_UnitScaler.y == 0.0f)
        m_UnitScaler.y = 1.0f;
}

const ax::pointf& CEditorCanvas::GetUnitScaler() const noexcept
{
    return m_UnitScaler;
}

void CEditorCanvas::SetUnitScaler(const ax::pointf& unitScaler) noexcept
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

ax::pointf CEditorCanvas::CalculateScaledUnit() const noexcept
{
    const auto& zoom = m_WindowCanvas.GetZoom();

    static const auto calculate = [](const auto& unit, const auto& zoom)
    {
        auto result = unit * zoom;

        while (result > unit * 2.0f)
            result /= 2.0f;

        while (result < unit * 0.5f)
            result *= 2.0f;

        return result;
    };

    return ax::pointf{
        calculate(m_UnitScaler.x, zoom.x),
        calculate(m_UnitScaler.y, zoom.y) };
}

ax::pointf CEditorCanvas::FromEditor(const ax::pointf& value) const noexcept
{
    const auto fromScreenValue = m_WindowCanvas.FromScreen(value);

    return ax::pointf{
        (fromScreenValue.x / m_UnitScaler.x),
        (-fromScreenValue.y / m_UnitScaler.y) };
}

ax::pointf CEditorCanvas::ToEditor(const ax::pointf& position) const noexcept
{
    return m_WindowCanvas.ToScreen(ax::pointf{
        (position.x * m_UnitScaler.x),
        (-position.y * m_UnitScaler.y) });
}