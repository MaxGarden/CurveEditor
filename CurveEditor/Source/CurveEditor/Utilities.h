#pragma  once
#if !defined(__UTILITIES_H__)

#include <imgui.h>

template<typename Type>
using VisitorType = std::function<void(Type&)>;

template<typename Type>
using ConstVisitorType = std::function<void(const Type&)>;

template<typename ContainerType, typename VisitorType>
inline void VisitPointersContainer(const ContainerType& container, const VisitorType& visitor) noexcept
{
    if (!visitor)
        return;

    for (auto& element : container)
    {
        if (element)
            visitor(*element);
    }
}

template<typename ContainerType, typename VisitorType>
inline void VisitObjectsContainer(const ContainerType& container, const VisitorType& visitor) noexcept
{
    if (!visitor)
        return;

    for (auto& element : container)
        visitor(element);
}

template<typename ContainerType, typename ValueType>
inline const auto RemoveFromContainer(ContainerType& container, const ValueType& value)
{
    const auto iterator = std::remove(container.begin(), container.end(), value);

    if (iterator == container.end())
        return false;

    container.erase(iterator, container.end());
    return true;
}

template<typename MapType, typename KeyType>
inline const auto RemoveFromMap(MapType& map, const KeyType& key)
{
    const auto iterator = map.find(key);

    if (iterator == map.end())
        return false;

    map.erase(iterator);
    return true;
}

#endif //__UTILITES_H__