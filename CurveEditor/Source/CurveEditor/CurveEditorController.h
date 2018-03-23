#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorProtocol.h"
#include "CurveEditorDataModel.h"
#include "EditorControllerBase.h"
#include "Style.h"

class ICurveEditorProtocol : public IEditorProtocol
{
public:
    virtual ~ICurveEditorProtocol() = default;

    virtual void OnViewCreate(const CCurveEditorViewControllerSharedPtr& viewController) = 0;
    virtual void OnViewDestroyed(const CCurveEditorViewControllerSharedPtr& viewController) = 0;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

template<typename DataModelType, typename ControllerType>
struct SCurveEditorStorage
{
    DataModelType m_DataModel;
    ControllerType m_Controller;
};

class IEditorViewController : public IEditorController
{
public:
    virtual ~IEditorViewController() override = default;


};

class IMainEditorControllerProtocol : public IEditorProtocol
{
    virtual void OnViewControllerCreated(const IEditorViewControllerSharedPtr& viewController) = 0;
    virtual void OnViewControllerDestroyed(const IEditorViewControllerSharedPtr& viewController) = 0;
};

class IMainEditorController : public IEditorController
{
public:
    virtual ~IMainEditorController() override = default;

    IEditorViewControllerSharedPtr CreateViewController(const IEditorDataModelSharedPtr& dataModel);
    //destroy

    //create view data model
};


class CCurveEditorController final : public CEditorControllerBase<IEditorController, CCurveEditorDataModel, ICurveEditorProtocol>
{
public:
    using ViewHandle = size_t;
    using SplineHandle = size_t;

public:
    CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory);
    virtual ~CCurveEditorController() override final = default;

    std::optional<ViewHandle> CreateView();
    bool DestroyView(const ViewHandle& handle);

    std::optional<SplineHandle> CreateSpline(std::string name);
    bool DestroySpline(const SplineHandle& handle);

    void VisitSplineControllers(const std::function<void(const ICurveEditorSplineControllerSharedPtr&)>& visitor) const noexcept;

    const SEditorStyle& GetEditorStyle() const noexcept;

private:
    virtual void OnDataModelChanged() override final;
    void RecreateSplineControllers();

private:
    using ViewStorage = SCurveEditorStorage<ICurveEditorViewDataModelSharedPtr, CCurveEditorViewControllerSharedPtr>;
    using SplineStorage = SCurveEditorStorage<ICurveEditorSplineDataModelSharedPtr, ICurveEditorSplineControllerSharedPtr>;

    std::map<ViewHandle, ViewStorage> m_EditorViewsStorages;
    std::map<SplineHandle,SplineStorage> m_SplineStorages;
    ICurveEditorSplineControllerFactory& m_SplineControllerFactory;
};

#endif //__CURVE_EDITOR_CONTROLLER_H__