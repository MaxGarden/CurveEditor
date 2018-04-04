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
    CurveEditorStyleColor_Count
};

struct SCurveEditorStyle
{
    ImColor Colors[CurveEditorStyleColor_Count];

    ImVec2 KnotSize;
    float SplineThickness;
    float TangentLineThickness;
    float TangentMarkerRadius;

    SCurveEditorStyle();
    ~SCurveEditorStyle() = default;
};

#endif //__STYLE_H__