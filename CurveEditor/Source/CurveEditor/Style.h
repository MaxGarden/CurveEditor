#pragma  once
#if !defined(__STYLE_H__)

#include <imgui.h>

enum EEditorStyleColor
{
    EditorStyleColor_Grid,
    EditorStyleColor_SmallGrid,
    EditorStyleColor_Background,
    EditorStyleColor_Count
};

struct SEditorStyle
{
    ImColor Colors[EditorStyleColor_Count];

    SEditorStyle();
    ~SEditorStyle() = default;
};

#endif //__STYLE_H__