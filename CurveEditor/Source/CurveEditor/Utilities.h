#pragma  once
#if !defined(__UTILITIES_H__)

#include <imgui.h>

namespace Utilities
{
    void TransformDrawListChannels(ImDrawList& drawList, int begin, int end, const ImVec2& preOffset, const ImVec2& scale, const ImVec2& postOffset);
    void TranslateAndClampDrawListClipRects(ImDrawList& drawList, int begin, int end, const ImVec2& offset);
    void GrowDrawListChannels(ImDrawList& drawList, int channelsCount);
}

#endif //__UTILITES_H__