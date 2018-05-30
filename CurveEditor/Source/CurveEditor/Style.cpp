#include "pch.h"
#include "Style.h"

#define DEFINE_COLOR(color, red, green, blue, alpha) Colors[color] = ImColor{ red, green, blue, alpha };

SCurveEditorStyle::SCurveEditorStyle()
{
    DEFINE_COLOR(CurveEditorStyleColor_Grid                 , 127, 127, 127,  50);
    DEFINE_COLOR(CurveEditorStyleColor_SmallGrid            , 127, 127, 127,  25);
    DEFINE_COLOR(CurveEditorStyleColor_Background           ,  32,  32,  32, 255);
    DEFINE_COLOR(CurveEditorStyleColor_Knot                 ,  60, 180, 255, 255);
    DEFINE_COLOR(CurveEditorStyleColor_TangentLine          , 255,   0,  60, 255);
    DEFINE_COLOR(CurveEditorStyleColor_TangentMarker        , 255, 180,  60, 255);
    DEFINE_COLOR(CurveEditorStyleColor_HoveringBorder       ,  50, 176, 255, 255);
    DEFINE_COLOR(CurveEditorStyleColor_SelectionBorder      , 255, 176,  50, 255);
    DEFINE_COLOR(CurveEditorStyleColor_SelectionRect        ,  50,  20, 255, 128);
    DEFINE_COLOR(CurveEditorStyleColor_SelectionRectBorder  ,  50,  20, 255, 255);
    DEFINE_COLOR(CurveEditorStyleColor_RulerText            , 127, 127, 127, 255);
    DEFINE_COLOR(CurveEditorStyleColor_RulerBorder          , 127, 127, 127,   0);
    DEFINE_COLOR(CurveEditorStyleColor_RulerBackground      , 127, 127, 127,   5);
    DEFINE_COLOR(CurveEditorStyleColor_RulerBigGraduation   ,   0,   0,   0, 255);
    DEFINE_COLOR(CurveEditorStyleColor_RulerSmallGraduation , 200, 200, 200, 255);
    DEFINE_COLOR(CurveEditorStyleColor_Helper               ,  80,  21, 238, 200);

    KnotSize = { 10.0f, 10.0f };
    SplineThickness = 3.0f;
    TangentLineThickness = 1.0f;
    TangentMarkerRadius = 5.0f;

    MinimumZoomLevel = -15.0f;
    MaximumZoomLevel = 20.0f;
    ZoomInMultipler = 1.3f;
    ZoomOutMultipler = 0.77f;

    DrawToolOnEveryView = true;

    HoveringBorderThickness = 4.0f;
    SelectionBorderThickness = 5.0f;

    SelectionViaIntersection = true;

    RulerSizeFactor = 2.0f;

    HelperBorderThickness = 4.0f;
}

std::optional<float> SCurveEditorStyle::GetEditorStyleFloat(ECurveEditorStyleFloat styleFloat) const noexcept
{
    switch (styleFloat)
    {
    case CurveEditorStyleFloat_SplineThickness:
        return SplineThickness;
    case CurveEditorStyleFloat_TangentLineThickness:
        return TangentLineThickness;
    case CurveEditorStyleFloat_TangentMarkerRadius:
        return TangentMarkerRadius;
    case CurveEditorStyleFloat_MinimumZoomLevel:
        return MinimumZoomLevel;
    case CurveEditorStyleFloat_MaximumZoomLevel:
        return MaximumZoomLevel;
    case CurveEditorStyleFloat_ZoomInMultipler:
        return ZoomInMultipler;
    case CurveEditorStyleFloat_ZoomOutMultipler:
        return ZoomOutMultipler;
    case CurveEditorStyleFloat_HoveringBorderThickness:
        return HoveringBorderThickness;
    case CurveEditorStyleFloat_SelectionBorderThickness:
        return SelectionBorderThickness;
    case CurveEditorStyleFloat_RulerSizeFactor:
        return RulerSizeFactor;
    case CurveEditorStyleFloat_HelperBorderThickness:
        return HelperBorderThickness;
    default:
        EDITOR_ASSERT(false);
        return std::nullopt;
    }
}

std::optional<ImVec2> SCurveEditorStyle::GetEditorStyleVec2(ECurveEditorStyleVec2 styleVec2) const noexcept
{
    switch (styleVec2)
    {
    case CurveEditorStyleFloatVec2_KnotSize:
        return KnotSize;
    default:
        EDITOR_ASSERT(false);
        return std::nullopt;
    }
}

std::optional<bool> SCurveEditorStyle::GetEditorStyleBool(ECurveEditorStyleBool styleBool) const noexcept
{
    switch (styleBool)
    {
    case CurveEditorStyleBool_DrawToolOnEveryView:
        return DrawToolOnEveryView;
    case CurveEditorStyleBool_SelectionViaIntersection:
        return SelectionViaIntersection;
    default:
        EDITOR_ASSERT(false);
        return std::nullopt;
    }
}
