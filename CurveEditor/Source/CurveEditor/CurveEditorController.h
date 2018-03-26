#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorListener.h"
#include "CurveEditorDataModel.h"
#include "EditorControllerBase.h"
#include "Style.h"

class ICurveEditorListener : public IEditorListener
{
public:
    virtual ~ICurveEditorListener() = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

template<typename DataModelType, typename ControllerType>
struct SCurveEditorStorage
{
    DataModelType m_DataModel;
    ControllerType m_Controller;
};

class CCurveEditorController final : public CEditorControllerBase<IEditorController, CCurveEditorDataModel, ICurveEditorListener>
{
public:
    using ViewHandle = size_t;
    using SplineHandle = size_t;

public:
    CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory);
    virtual ~CCurveEditorController() override final = default;

    std::optional<SplineHandle> CreateSpline(std::string name);
    bool DestroySpline(const SplineHandle& handle);

    bool SetActiveTool(ICurveEditorToolSharedPtr&& tool) noexcept;
    const ICurveEditorToolSharedPtr& GetActiveTool() const noexcept;

    void VisitSplineControllers(const std::function<void(const ICurveEditorSplineControllerSharedPtr&)>& visitor) const noexcept;

    const SEditorStyle& GetEditorStyle() const noexcept;

private:
    virtual void OnDataModelChanged() override final;
    void RecreateSplineControllers();

private:
    using SplineStorage = SCurveEditorStorage<ICurveEditorSplineDataModelSharedPtr, ICurveEditorSplineControllerSharedPtr>;

    std::map<SplineHandle,SplineStorage> m_SplineStorages;
    ICurveEditorSplineControllerFactory& m_SplineControllerFactory;
    ICurveEditorToolSharedPtr m_ActiveTool;
};

#endif //__CURVE_EDITOR_CONTROLLER_H__