#include "pch.h"
#include "SaveManager.h"
#include "EditorContext.h"
#include "Serializers/CurveEditorDataModelSerializer.h"
#include <QFile>

class CSaveManager final : public ISaveManager
{
public:
    CSaveManager() = default;
    virtual ~CSaveManager() override final = default;

    virtual void SetSerializer(IEditorSerializerUniquePtr&& serializer) noexcept override final;
    virtual IEditorSerializer* GetSerializer() const noexcept override final;

    virtual void SetSaveFilePath(std::optional<std::string>&& filePath) noexcept override final;
    virtual const std::optional<std::string>& GetSaveFilePath() const noexcept override final;

    virtual bool Save(const IEditorContext& editorContext) const override final;
    virtual bool Load(IEditorContext& editorContext) const override final;

private:
    bool Save(const std::string& filePath, const SerializedData& serializedData) const;
    bool Load(const std::string& filePath, SerializedData& serializedData) const;

private:
    IEditorSerializerUniquePtr m_Serializer;
    std::optional<std::string> m_SaveFile;
};

void CSaveManager::SetSerializer(IEditorSerializerUniquePtr&& serializer) noexcept
{
    m_Serializer = std::move(serializer);
}

IEditorSerializer* CSaveManager::GetSerializer() const noexcept
{
    return m_Serializer.get();
}

void CSaveManager::SetSaveFilePath(std::optional<std::string>&& filePath) noexcept
{
    m_SaveFile = std::move(filePath);
}

const std::optional<std::string>& CSaveManager::GetSaveFilePath() const noexcept
{
    return m_SaveFile;
}

bool CSaveManager::Save(const IEditorContext& editorContext) const
{
    EDITOR_ASSERT(m_SaveFile);
    if (!m_SaveFile)
        return false;

    EDITOR_ASSERT(m_Serializer);
    if (!m_Serializer)
        return false;

    const auto& editorDataModel = editorContext.GetDataModel();
    EDITOR_ASSERT(editorDataModel);
    if (!editorDataModel)
        return false;

    SerializedData serializedData;
    const auto result = m_Serializer->Serialize(*editorDataModel, serializedData);
    EDITOR_ASSERT(result);
    if (!result)
        return false;

    return Save(*m_SaveFile, serializedData);
}

bool CSaveManager::Load(IEditorContext& editorContext) const
{
    EDITOR_ASSERT(m_SaveFile);
    if (!m_SaveFile)
        return false;

    EDITOR_ASSERT(m_Serializer);
    if (!m_Serializer)
        return false;

    SerializedData serializedData;

    const auto result = Load(*m_SaveFile, serializedData);
    EDITOR_ASSERT(result);
    if (!result)
        return false;

    auto editorDataModel = m_Serializer->Load(serializedData);
    EDITOR_ASSERT(editorDataModel);
    if (!editorDataModel)
        return false;

    editorContext.SetDataModel(std::move(editorDataModel));
    return true;
}

bool CSaveManager::Save(const std::string& filePath, const SerializedData& serializedData) const
{
    QFile file{ filePath.c_str() };

    if (!file.open(QIODevice::WriteOnly| QIODevice::Text))
        return false;

    const auto serializedDataSize = serializedData.size();
    const auto savedDataSize = file.write(serializedData.data(), serializedDataSize);

    EDITOR_ASSERT(savedDataSize == serializedDataSize);
    if (savedDataSize != serializedDataSize)
        return false;

    file.close();
    return true;
}

bool CSaveManager::Load(const std::string& filePath, SerializedData& serializedData) const
{
    QFile file{ filePath.c_str() };

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    const auto fileSize = file.size();
    serializedData.resize(fileSize);
    file.read(serializedData.data(), serializedData.size());

    file.close();
    return true;
}

ISaveManager& ISaveManager::GetInstance() noexcept
{
    static CSaveManager instance;
    return instance;
}