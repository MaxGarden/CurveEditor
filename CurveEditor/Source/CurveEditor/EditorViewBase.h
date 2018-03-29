#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_BASE_H__)

#include "EditorView.h"

template<typename SuperClass, typename ControllerType>
class CEditorViewBase : public SuperClass
{
public:
    CEditorViewBase() = default;
    virtual ~CEditorViewBase() override = default;

    virtual void OnFrame() override;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override;

    const std::shared_ptr<ControllerType>& GetController() const noexcept;

protected:
    virtual void OnControllerChanged();

    using Super = CEditorViewBase<SuperClass, ControllerType>;

private:
    std::shared_ptr<ControllerType> m_Controller;
};

#include "EditorViewBase.inl"

#endif //__CURVE_EDITOR_VIEW_BASE_H__