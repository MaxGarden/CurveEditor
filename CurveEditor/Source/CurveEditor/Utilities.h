#pragma  once
#if !defined(__UTILITIES_H__)

#include <imgui.h>

namespace Utilities
{
    void TransformDrawListChannels(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& preOffset, const ImVec2& scale, const ImVec2& postOffset);
    void TranslateAndClampDrawListClipRects(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& offset);
    void GrowDrawListChannels(ImDrawList& drawList, size_t channelsCount);

    size_t GetBackgroundChannelStart() noexcept;
    size_t GetBackgroundChannelCount() noexcept;
}

#endif //__UTILITES_H__