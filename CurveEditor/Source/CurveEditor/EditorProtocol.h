#pragma  once
#if !defined(__EDITOR_PROTOCOL_H__)

using EditorProtocolHandle = size_t;
const EditorProtocolHandle InvalidProtocolHandle = 0;

class IEditorProtocol
{
public:
    virtual ~IEditorProtocol() = default;
};

#endif //__EDITOR_PROTOCOL_H__