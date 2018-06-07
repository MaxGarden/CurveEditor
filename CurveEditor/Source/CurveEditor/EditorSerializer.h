#pragma once
#if !defined(__EDITOR_SERIALIZER_H__)

using SerializedData = std::string;

class IEditorSerializer
{
public:
    virtual ~IEditorSerializer() = default;

    virtual bool Serialize(IEditorDataModel& dataModel, SerializedData& serializedData) = 0;
    virtual IEditorDataModelUniquePtr Load(const SerializedData& sertializedData) = 0;
};

#endif //__EDITOR_SERIALIZER_H__