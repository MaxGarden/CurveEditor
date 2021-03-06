#pragma once
#if !defined(__CURVE_EDITOR_VIEW_BASE_H__)

#include "EditorView.h"

template<typename SuperClass, typename ControllerType>
class CEditorViewBase : public SuperClass
{
public:
    CEditorViewBase() = default;
    virtual ~CEditorViewBase() override = default;

    virtual void OnFrame() override;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) override;

protected:
    virtual void OnControllerChanged();
    virtual IEditorListenerUniquePtr CreateListener();

    const std::shared_ptr<ControllerType>& GetController() const noexcept;

    using Super = CEditorViewBase<SuperClass, ControllerType>;

private:
    std::shared_ptr<ControllerType> m_Controller;
    EditorListenerHandle m_ListenerHandle;
};

#include "EditorViewBase.inl"

#endif //__CURVE_EDITOR_VIEW_BASE_H__