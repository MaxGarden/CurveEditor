#pragma  once
#if !defined(__CANVAS_H__)

#include <imgui.h>
#include <ax/ax.h>

class CWindowCanvas
{
public:
    CWindowCanvas() noexcept;
    CWindowCanvas(const ImVec2& position, const ImVec2& size, const ImVec2& scale, const ImVec2& origin) noexcept;

    const ImVec2& GetWindowScreenPosition() const noexcept;
    const ImVec2& GetWindowScreenSize() const noexcept;
    const ImVec2& GetClientOrigin() const noexcept;
    const ImVec2& GetClientSize() const noexcept;
    const ImVec2& GetZoom() const noexcept;
    const ImVec2& GetInvertZoom() const noexcept;

    ax::rectf GetVisibleBounds(bool zoom = false) const noexcept;

    ImVec2 FromScreen(const ImVec2& point, bool zoom = false) const noexcept;
    ImVec2 ToScreen(const ImVec2& point) const noexcept;
    ImVec2 FromClient(const ImVec2& point) const noexcept;
    ImVec2 ToClient(const ImVec2& point) const noexcept;

private:
    ImVec2 m_WindowScreenPosition;
    ImVec2 m_WindowScreenSize;
    ImVec2 m_ClientOrigin;
    ImVec2 m_ClientSize;
    ImVec2 m_Zoom;
    ImVec2 m_InvertZoom;

};

class CEditorCanvas
{
public:
    CEditorCanvas(const ImVec2& unitScaler);
    ~CEditorCanvas() = default;

    const ImVec2& GetUnitScaler() const noexcept;
    void SetUnitScaler(const ImVec2& unitScaler) noexcept;

    CWindowCanvas& GetWindowCanvas() noexcept;
    const CWindowCanvas& GetWindowCanvas() const noexcept;

    ImVec2 FromEditor(const ImVec2& value) const noexcept;
    ImVec2 ToEditor(const ImVec2& position) const noexcept;

private:
    ImVec2 m_UnitScaler;
    CWindowCanvas m_WindowCanvas;
};

#endif //__CANVAS_H__