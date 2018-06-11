#pragma once
#if !defined(__SAVE_MANAGER_H__)

#include "ActionHandlerBase.h"

class ISaveManager
{
public:
    virtual ~ISaveManager() = default;

    virtual void SetSerializer(IEditorSerializerUniquePtr&& serializer) noexcept = 0;
    virtual IEditorSerializer* GetSerializer() const noexcept = 0;

    virtual void SetSaveFilePath(std::optional<std::string>&& filePath) noexcept = 0;
    virtual const std::optional<std::string>& GetSaveFilePath() const noexcept = 0;

    virtual bool Save(const IEditorContext& editorContext) const = 0;
    virtual bool Load(IEditorContext& editorContext) const = 0;

    static ISaveManager& GetInstance() noexcept;
};

#endif //__SAVE_MANAGER_H__