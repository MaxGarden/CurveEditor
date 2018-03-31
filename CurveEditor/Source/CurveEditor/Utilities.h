#pragma  once
#if !defined(__UTILITIES_H__)

#include <imgui.h>

namespace ImGuiUtilities
{
    void TransformDrawListChannels(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& preOffset, const ImVec2& scale, const ImVec2& postOffset);
    void TranslateAndClampDrawListClipRects(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& offset);
    void GrowDrawListChannels(ImDrawList& drawList, size_t channelsCount);

    size_t GetBackgroundChannelStart() noexcept;
    size_t GetBackgroundChannelCount() noexcept;
}

template<typename ContainerType, typename VisitorType>
inline void VisitContainer(const ContainerType& container, const VisitorType& visitor) noexcept
{
    if (!visitor)
        return;

    for (auto& element : container)
    {
        if (element)
            visitor(*element);
    }
}

#endif //__UTILITES_H__