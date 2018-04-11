#include "pch.h"
#include "Style.h"

#define DEFINE_COLOR(color, red, green, blue, alpha) Colors[color] = ImColor(red, green, blue, alpha)

SCurveEditorStyle::SCurveEditorStyle()
{
    DEFINE_COLOR(CurveEditorStyleColor_Grid          , 127, 127, 127, 050);
    DEFINE_COLOR(CurveEditorStyleColor_SmallGrid     , 127, 127, 127, 025);
    DEFINE_COLOR(CurveEditorStyleColor_Background    , 032, 032, 032, 255);
    DEFINE_COLOR(CurveEditorStyleColor_Knot          , 060, 180, 255, 255);
    DEFINE_COLOR(CurveEditorStyleColor_TangentLine   , 255, 000, 060, 255);
    DEFINE_COLOR(CurveEditorStyleColor_TangentMarker , 255, 180, 060, 255);

    KnotSize = { 10.0f, 10.0f };
    SplineThickness = 3.0f;
    TangentLineThickness = 1.0f;
    TangentMarkerRadius = 5.0f;

    MinimumZoomLevel = -15.0f;
    MaximumZoomLevel = 20.0f;
    ZoomInMultipler = 1.3f;
    ZoomOutMultipler = 0.77f;
}