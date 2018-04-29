#pragma  once
#if !defined(__CANVAS_H__)

#include <ax/ax.h>

class CWindowCanvas
{
public:
    CWindowCanvas() noexcept;
    CWindowCanvas(const ax::pointf& position, const ax::sizef& size, const ax::pointf& zoom, const ax::pointf& origin) noexcept;

    const ax::pointf& GetWindowScreenPosition() const noexcept;
    void SetWindowScreenPosition(const ax::pointf& position) noexcept;

    const ax::sizef& GetWindowScreenSize() const noexcept;
    void SetWindowScreenSize(const ax::sizef& size) noexcept;

    const ax::sizef& GetClientSize() const noexcept;

    const ax::pointf& GetClientOrigin() const noexcept;
    void SetClientOrigin(const ax::pointf& origin) noexcept;

    const ax::pointf& GetZoom() const noexcept;
    void SetZoom(const ax::pointf& zoom) noexcept;

    const ax::pointf& GetInvertZoom() const noexcept;

    ax::rectf CalculateVisibleBounds() const noexcept;

    ax::pointf FromScreen(const ax::pointf& point) const noexcept;
    ax::pointf ToScreen(const ax::pointf& point) const noexcept;
    ax::pointf FromClient(const ax::pointf& point) const noexcept;
    ax::pointf ToClient(const ax::pointf& point) const noexcept;

private:
    void RefreshClientSize() noexcept;

private:
    ax::pointf m_WindowScreenPosition;
    ax::sizef m_WindowScreenSize;
    ax::pointf m_ClientOrigin;
    ax::sizef m_ClientSize;
    ax::pointf m_Zoom;
    ax::pointf m_InvertZoom;

};

class CEditorCanvas
{
public:
    CEditorCanvas(const ax::pointf& unitScaler);
    ~CEditorCanvas() = default;

    const ax::pointf& GetUnitScaler() const noexcept;
    void SetUnitScaler(const ax::pointf& unitScaler) noexcept;

    CWindowCanvas& GetWindowCanvas() noexcept;
    const CWindowCanvas& GetWindowCanvas() const noexcept;

    ax::pointf CalculateScaledUnit() const noexcept;

    ax::pointf FromEditor(const ax::pointf& value, bool screenTranslation = true) const noexcept;
    ax::pointf ToEditor(const ax::pointf& position, bool screenTranslation = true) const noexcept;

private:
    ax::pointf m_UnitScaler;
    CWindowCanvas m_WindowCanvas;
};

#endif //__CANVAS_H__