#include "pch.h"
#include "Utilities.h"
#include <bitset>
#include <algorithm>

void ImGuiUtilities::TransformDrawListChannels(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& preOffset, const ImVec2& scale, const ImVec2& postOffset)
{
    const auto innerTransform = [&vertexBuffer = drawList.VtxBuffer, &indexBuffer = drawList.IdxBuffer, &preOffset, &scale, &postOffset](const auto& cmdBuffer)
    {
        const auto idxRead = indexBuffer.Data;

        std::bitset<65536> indexMap;

        auto minIndex = 65536;
        auto maxIndex = 0;
        auto indexOffset = 0;

        for (const auto& cmd : cmdBuffer)
        {
            const auto idxCount = cmd.ElemCount;

            if (idxCount == 0)
                continue;

            for (auto i = 0u; i < idxCount; ++i)
            {
                int idx = idxRead[indexOffset + i];
                indexMap.set(idx);

                if (minIndex > idx)
                    minIndex = idx;
                if (maxIndex < idx)
                    maxIndex = idx;
            }

            indexOffset += idxCount;
        }

        ++maxIndex;

        for (auto idx = minIndex; idx < maxIndex; ++idx)
        {
            if (!indexMap.test(idx))
                continue;

            auto& vertex = vertexBuffer.Data[idx];

            vertex.pos.x = (vertex.pos.x + preOffset.x) * scale.x + postOffset.x;
            vertex.pos.y = (vertex.pos.y + preOffset.y) * scale.y + postOffset.y;
        }
    };

    const auto lastCurrentChannel = drawList._ChannelsCurrent;
    if (lastCurrentChannel != 0)
        drawList.ChannelsSetCurrent(0);

    if (begin == 0 && begin != end)
    {
        innerTransform( drawList.CmdBuffer);
        ++begin;
    }

    for (auto channelIndex = begin; channelIndex < end; ++channelIndex)
    {
        auto& channel = drawList._Channels[static_cast<int>(channelIndex)];
        innerTransform(channel.CmdBuffer);
    }

    if (lastCurrentChannel != 0)
        drawList.ChannelsSetCurrent(lastCurrentChannel);
}

void ImGuiUtilities::TranslateAndClampDrawListClipRects(ImDrawList& drawList, size_t begin, size_t end, const ImVec2& offset)
{
    int lastCurrentChannel = drawList._ChannelsCurrent;
    if (lastCurrentChannel != 0)
        drawList.ChannelsSetCurrent(0);

    const auto innerTransform = [&clipRect = drawList._ClipRectStack.back(), &offset](auto& commandBuffer)
    {
        for (auto& command : commandBuffer)
        {
            command.ClipRect.x = std::max(command.ClipRect.x + offset.x, clipRect.x);
            command.ClipRect.y = std::max(command.ClipRect.y + offset.y, clipRect.y);
            command.ClipRect.z = std::min(command.ClipRect.z + offset.x, clipRect.z);
            command.ClipRect.w = std::min(command.ClipRect.w + offset.y, clipRect.w);
        }
    };

    if (begin == 0 && begin != end)
    {
        innerTransform(drawList.CmdBuffer);
        ++begin;
    }

    for (auto channelIndex = begin; channelIndex < end; ++channelIndex)
    {
        auto& channel = drawList._Channels[static_cast<int>(channelIndex)];
        innerTransform(channel.CmdBuffer);
    }

    if (lastCurrentChannel != 0)
        drawList.ChannelsSetCurrent(lastCurrentChannel);
}

void ImGuiUtilities::GrowDrawListChannels(ImDrawList& drawList, size_t channelsCount)
{
    EDITOR_ASSERT(drawList._ChannelsCount <= channelsCount);
    const auto previousChannelsCount = drawList._Channels.Size;
    if (previousChannelsCount < channelsCount)
        drawList._Channels.resize(static_cast<int>(channelsCount));

    const auto previousUsedChannelsCount = drawList._ChannelsCount;
    drawList._ChannelsCount = static_cast<int>(channelsCount);

    if (previousChannelsCount == 0)
        memset(&drawList._Channels[0], 0, sizeof(ImDrawChannel));

    for (int i = previousUsedChannelsCount; i < channelsCount; i++)
    {
        if (i >= previousChannelsCount)
            new (&drawList._Channels[i]) ImDrawChannel();
        else
        {
            drawList._Channels[i].CmdBuffer.resize(0);
            drawList._Channels[i].IdxBuffer.resize(0);
        }
        if (drawList._Channels[i].CmdBuffer.Size == 0)
        {
            ImDrawCmd drawCommand;
            drawCommand.ClipRect = drawList._ClipRectStack.back();
            drawCommand.TextureId = drawList._TextureIdStack.back();
            drawList._Channels[i].CmdBuffer.push_back(drawCommand);
        }
    }
}

size_t ImGuiUtilities::GetBackgroundChannelStart() noexcept
{
    return 0;
}

size_t ImGuiUtilities::GetBackgroundChannelCount() noexcept
{
    return 1;
}