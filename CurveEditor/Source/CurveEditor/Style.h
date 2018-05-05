#pragma  once
#if !defined(__STYLE_H__)

#include <imgui.h>

enum ECurveEditorStyleColor
{
    CurveEditorStyleColor_Grid,
    CurveEditorStyleColor_SmallGrid,
    CurveEditorStyleColor_Background,
    CurveEditorStyleColor_Knot,
    CurveEditorStyleColor_TangentLine,
    CurveEditorStyleColor_TangentMarker,
    CurveEditorStyleColor_HoveringBorder,
    CurveEditorStyleColor_SelectionBorder,
    CurveEditorStyleColor_SelectionRect,
    CurveEditorStyleColor_SelectionRectBorder,
    CurveEditorStyleColor_Count
};

enum ECurveEditorStyleFloat
{
    CurveEditorStyleFloat_SplineThickness,
    CurveEditorStyleFloat_TangentLineThickness,
    CurveEditorStyleFloat_TangentMarkerRadius,

    CurveEditorStyleFloat_MinimumZoomLevel,
    CurveEditorStyleFloat_MaximumZoomLevel,
    CurveEditorStyleFloat_ZoomInMultipler,
    CurveEditorStyleFloat_ZoomOutMultipler,

    CurveEditorStyleFloat_HoveringBorderThickness,

    CurveEditorStyleFloat_SelectionBorderThickness
};

enum ECurveEditorStyleVec2
{
    CurveEditorStyleFloatVec2_KnotSize
};

enum ECurveEditorStyleBool
{
    CurveEditorStyleBool_DrawToolOnEveryView,
    CurveEditorStyleBool_SelectionViaIntersection
};

struct SCurveEditorStyle
{
    std::array<ImColor, CurveEditorStyleColor_Count> Colors;

    ImVec2 KnotSize;
    float SplineThickness;
    float TangentLineThickness;
    float TangentMarkerRadius;

    float MinimumZoomLevel;
    float MaximumZoomLevel;
    float ZoomInMultipler;
    float ZoomOutMultipler;

    bool DrawToolOnEveryView;

    float HoveringBorderThickness;
    float SelectionBorderThickness;

    bool SelectionViaIntersection;

    SCurveEditorStyle();
    ~SCurveEditorStyle() = default;

    std::optional<float> GetEditorStyleFloat(ECurveEditorStyleFloat styleFloat) const noexcept;
    std::optional<ImVec2> GetEditorStyleVec2(ECurveEditorStyleVec2 styleVec2) const noexcept;
    std::optional<bool> GetEditorStyleBool(ECurveEditorStyleBool styleBool) const noexcept;
};

#endif //__STYLE_H__