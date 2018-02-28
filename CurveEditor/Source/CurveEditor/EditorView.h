#pragma  once
#if !defined(__EDITOR_VIEW_H__)

#include <memory>
#include <vector>

class IEditorViewComponent
{
public:
    virtual ~IEditorViewComponent() = default;

    virtual void OnFrame() = 0;
};

class IEditorView
{
public:
    virtual ~IEditorView() = default;

    virtual void OnFrame() = 0;

    virtual bool AddComponent(std::unique_ptr<IEditorViewComponent>&& component) = 0;
};

class IEditorViewFactory
{
public:
    virtual ~IEditorViewFactory() = default;
    virtual std::unique_ptr<IEditorView> Create() = 0;
};

class CEditorView : public IEditorView
{
public:
    CEditorView() = default;
    ~CEditorView() override = default;

    virtual void OnFrame() override;
    virtual bool AddComponent(std::unique_ptr<IEditorViewComponent>&& component) override final;

private:
    std::vector<std::unique_ptr<IEditorViewComponent>> m_Components;
};

#endif //__EDITOR_VIEW_H__