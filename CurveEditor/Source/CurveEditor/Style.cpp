#include "Style.h"

#define DEFINE_COLOR(color, red, green, blue, alpha) Colors[color] = ImColor(red, green, blue, alpha)

SEditorStyle::SEditorStyle()
{
    DEFINE_COLOR(EditorStyleColor_Grid          , 127, 127, 127, 50);
    DEFINE_COLOR(EditorStyleColor_SmallGrid     , 127, 127, 127, 25);
    DEFINE_COLOR(EditorStyleColor_Background    , 32, 32, 32, 255);
}