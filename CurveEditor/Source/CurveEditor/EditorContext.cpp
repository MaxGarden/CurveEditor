#include "pch.h"
#include "EditorContext.h"

class CEditorContext final : public IEditorContext
{
public:
    CEditorContext() = default;
    virtual ~CEditorContext() override final = default;

    virtual void SetViewFactory(IEditorViewFactoryUniquePtr&& factory) noexcept override final;
    virtual const IEditorViewFactoryUniquePtr& GetViewFactory() const noexcept override final;

    virtual void SetController(IEditorControllerSharedPtr&& controller) noexcept override final;
    virtual const IEditorControllerSharedPtr& GetController() const noexcept override final;

    virtual void SetDataModel(IEditorDataModelSharedPtr&& dataModel) noexcept override final;
    virtual const IEditorDataModelSharedPtr& GetDataModel() const noexcept override final;

    virtual IEditorViewSharedPtr AddView() override final;
    virtual bool RemoveView(const IEditorViewSharedPtr& view) override final;

private:
    void VisitViews(const std::function<void(IEditorView&)>& visitor);

private:
    IEditorDataModelSharedPtr m_DataModel;
    IEditorControllerSharedPtr m_Controller;

    std::vector<IEditorViewSharedPtr> m_Views;

    IEditorViewFactoryUniquePtr m_ViewFactory;
};

void CEditorContext::SetViewFactory(IEditorViewFactoryUniquePtr&& factory) noexcept
{
    m_ViewFactory = std::move(factory);
}

const IEditorViewFactoryUniquePtr& CEditorContext::GetViewFactory() const noexcept
{
    return m_ViewFactory;
}

void CEditorContext::SetController(IEditorControllerSharedPtr&& controller) noexcept
{
	if (m_Controller = std::move(controller))
		m_Controller->SetDataModel(GetDataModel());

    auto result = true;
    VisitViews([&controller, &result](auto& view)
    {
        result &= view.SetController(controller);
    });

    EDITOR_ASSERT(result && "Cannot set controller in views");
}

const IEditorControllerSharedPtr& CEditorContext::GetController() const noexcept
{
    return m_Controller;
}

void CEditorContext::SetDataModel(IEditorDataModelSharedPtr&& dataModel) noexcept
{
    m_DataModel = std::move(dataModel);

	if(const auto& controller = GetController())
		controller->SetDataModel(m_DataModel);
}

const IEditorDataModelSharedPtr& CEditorContext::GetDataModel() const noexcept
{
    return m_DataModel;
}

IEditorViewSharedPtr CEditorContext::AddView()
{
    if (!m_ViewFactory || !m_Controller)
        return nullptr;

    const auto editorView = IEditorViewSharedPtr(m_ViewFactory->Create());
    if (!editorView)
        return nullptr;

    auto isVaild = true;
    isVaild &= editorView->SetController(m_Controller);

    if (!isVaild)
        return nullptr;

    m_Views.emplace_back(editorView);

    return editorView;
}

bool CEditorContext::RemoveView(const IEditorViewSharedPtr& view)
{
    const auto iterator = std::find(m_Views.begin(), m_Views.end(), view);
    if (iterator == m_Views.end())
        return false;

    m_Views.erase(iterator);
    return true;
}

void CEditorContext::VisitViews(const std::function<void(IEditorView &)>& visitor)
{
    if (!visitor)
        return;

    for (const auto& view : m_Views)
    {
        if (view)
            visitor(*view);
    }
}

IEditorContextUniquePtr IEditorContext::CreateContext()
{
    return std::make_unique<CEditorContext>();
}