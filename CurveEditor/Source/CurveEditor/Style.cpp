#include "pch.h"
#include "Style.h"

#define DEFINE_COLOR(color, red, green, blue, alpha) Colors[color] = ImColor(red, green, blue, alpha)

SEditorStyle::SEditorStyle()
{
    DEFINE_COLOR(EditorStyleColor_Grid          , 127, 127, 127, 050);
    DEFINE_COLOR(EditorStyleColor_SmallGrid     , 127, 127, 127, 025);
    DEFINE_COLOR(EditorStyleColor_Background    , 032, 032, 032, 255);
    DEFINE_COLOR(EditorStyleColor_Knot          , 127, 032, 032, 255);

    KnotSize = { 10.0f, 10.0f };
}