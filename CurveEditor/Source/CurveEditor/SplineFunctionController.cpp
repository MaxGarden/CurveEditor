#include "pch.h"
#include "SplineFunctionController.h"
#include "CurveEditorController.h"
#include "CurveEditorSplineListenerBase.h"
#include "KnotController.h"
#include "TangentController.h"
#include "CurveController.h"

class CCurveEditorFunctionSplineController final : public CEditorControllerBase<ICurveEditorFunctionSplineController, ICurveEditorSplineDataModel, ICurveEditorSplineListener>
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    virtual const SplineID& GetID() const noexcept override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual void VisitKnotsControllers(const VisitorCopyType<ICurveEditorKnotControllerSharedPtr>& visitor) const override final;
    virtual void VisitTangentsControllers(const VisitorCopyType<ICurveEditorTangentControllerSharedPtr>& visitor) const override final;
    virtual void VisitCurvesControllers(const VisitorCopyType<ICurveEditorCurveControllerSharedPtr>& visitor) const override final;

    void OnKnotInserted(size_t controlPointIndex);
    void OnKnotRemoved(size_t controlPointIndex);

protected:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    virtual size_t GetKnotsCount() const noexcept;
    virtual size_t GetTangentsCount() const noexcept;
    virtual size_t GetCurvesCount() const noexcept;

    const std::vector<ax::pointf>& GetControlPoints() const noexcept;

    void CreateControllers();
    void DestroyControllers();
    void OnSplineModified() noexcept;
    void SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept;

    ICurveEditorKnotControllerPrivateSharedPtr AddKnotController(size_t knotIndex);
    ICurveEditorTangentControllerPrivateSharedPtr AddTangentController(size_t tangentIndex);
    ICurveEditorCurveControllerPrivateSharedPtr AddCurveController(size_t curveIndex);

private:
    std::vector<ICurveEditorKnotControllerPrivateSharedPtr> m_KnotsControllers;
    std::vector<ICurveEditorTangentControllerPrivateSharedPtr> m_TangentsControllers;
    std::vector<ICurveEditorCurveControllerPrivateSharedPtr> m_CurvesControllers;
};

class CCurveEditorFunctionSplineControllerListener final : public CCurveEditorSplineDataModelListenerBase
{
public:
    CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController);
    virtual ~CCurveEditorFunctionSplineControllerListener() override final = default;

    virtual void OnKnotInserted(size_t controlPointIndex) override final;
    virtual void OnKnotRemoved(size_t controlPointIndex) override final;

private:
    CCurveEditorFunctionSplineController& m_FunctionSplineController;
};

CCurveEditorFunctionSplineControllerListener::CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController) :
    m_FunctionSplineController(functionSplineController)
{
}

void CCurveEditorFunctionSplineControllerListener::OnKnotInserted(size_t controlPointIndex)
{
    m_FunctionSplineController.OnKnotInserted(controlPointIndex);
}

void CCurveEditorFunctionSplineControllerListener::OnKnotRemoved(size_t controlPointIndex)
{
    m_FunctionSplineController.OnKnotRemoved(controlPointIndex);
}

const SplineID& CCurveEditorFunctionSplineController::GetID() const noexcept
{
    static const SplineID null = ICurveEditorSplineDataModel::InvalidSplineID();

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetID();

    return null;
}

const SplineColor& CCurveEditorFunctionSplineController::GetColor() const noexcept
{
    static const SplineColor null = {};

    if (const auto dataModel = GetDataModel())
        return dataModel->GetColor();

    return null;
}

void CCurveEditorFunctionSplineController::VisitKnotsControllers(const VisitorCopyType<ICurveEditorKnotControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_KnotsControllers, visitor);
}

void CCurveEditorFunctionSplineController::VisitTangentsControllers(const VisitorCopyType<ICurveEditorTangentControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_TangentsControllers, visitor);
}

void CCurveEditorFunctionSplineController::VisitCurvesControllers(const VisitorCopyType<ICurveEditorCurveControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_CurvesControllers, visitor);
}

void CCurveEditorFunctionSplineController::OnKnotInserted(size_t)
{
    //TODO
    EDITOR_ASSERT(false);
}

void CCurveEditorFunctionSplineController::OnKnotRemoved(size_t)
{
    //TODO
    EDITOR_ASSERT(false);
}

void CCurveEditorFunctionSplineController::OnSplineModified() noexcept
{
    const auto& dataModel = GetDataModel();
    if(!dataModel)
        return;

    SortControlPoints(dataModel->GetControlPoints());
}

void CCurveEditorFunctionSplineController::OnDataModelChanged()
{
    DestroyControllers();
    CreateControllers();
}

IEditorListenerUniquePtr CCurveEditorFunctionSplineController::CreateListener()
{
    return std::make_unique<CCurveEditorFunctionSplineControllerListener>(*this);
}

size_t CCurveEditorFunctionSplineController::GetKnotsCount() const noexcept
{
    const auto curvesCount = GetCurvesCount();
    if (curvesCount == 0)
        return 0;

    return curvesCount + 1;
}

size_t CCurveEditorFunctionSplineController::GetTangentsCount() const noexcept
{
    return GetCurvesCount() * 2;
}

size_t CCurveEditorFunctionSplineController::GetCurvesCount() const noexcept
{
    const auto controlPointsPerCurve = ControlPointsPerCurve();

    const auto controlPointsCount = GetControlPoints().size();
    if (controlPointsCount < controlPointsPerCurve)
        return 0;

    return 1 + (controlPointsCount - controlPointsPerCurve) / (controlPointsPerCurve - 1);
}

const std::vector<ax::pointf>& CCurveEditorFunctionSplineController::GetControlPoints() const noexcept
{
    static const std::vector<ax::pointf> null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetControlPoints();

    return null;
}

void CCurveEditorFunctionSplineController::CreateControllers()
{
    const auto createControllers = [this](const auto controllersCount, const auto addMethod, const auto listenerMethod)
    {
        for (auto i = 0u; i < controllersCount; ++i)
        {
            const auto addedController = (this->*addMethod)(i);
            EDITOR_ASSERT(addedController);
            if (!addedController)
                continue;

            NotifyListeners(listenerMethod, addedController);
        }
    };

    createControllers(GetKnotsCount(), &CCurveEditorFunctionSplineController::AddKnotController, &ICurveEditorSplineListener::OnKnotCreated);
    createControllers(GetTangentsCount(), &CCurveEditorFunctionSplineController::AddTangentController, &ICurveEditorSplineListener::OnTangentCreated);
    createControllers(GetCurvesCount(), &CCurveEditorFunctionSplineController::AddCurveController, &ICurveEditorSplineListener::OnCurveCreated);
}

void CCurveEditorFunctionSplineController::DestroyControllers()
{
    const auto notifyDestroyControllers = [this](const auto& container, const auto method)
    {
        VisitObjectsContainer(container, [&](const auto& controlller)
        {
            NotifyListeners(method, controlller);
        });
    };

    notifyDestroyControllers(m_CurvesControllers, &ICurveEditorSplineListener::OnCurveDestroyed);
    notifyDestroyControllers(m_TangentsControllers, &ICurveEditorSplineListener::OnTangentCreated);
    notifyDestroyControllers(m_KnotsControllers, &ICurveEditorSplineListener::OnKnotDestroyed);

    m_CurvesControllers.clear();
    m_TangentsControllers.clear();
    m_KnotsControllers.clear();
}

void CCurveEditorFunctionSplineController::SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept
{
    static const auto prediction = [](const auto& first, const auto& second)
    {
        return first.x < second.x;
    };

    std::sort(controlPoints.begin(), controlPoints.end(), prediction);
}

template<typename ControllerType, typename DataModelType, typename Method>
static auto CreateSplineControllerComponent(const DataModelType& dataModel, const Method setMethod, size_t controlPointIndex)
{
    auto result = std::shared_ptr<ControllerType>(ControllerType::Create());

    if (result)
    {
        auto isValid = true;

        isValid &= result->SetDataModel(dataModel);
        isValid &= (result.get()->*setMethod)(controlPointIndex);
        EDITOR_ASSERT(isValid);

        if (!isValid)
            result.reset();
    }

    return result;
}

ICurveEditorKnotControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddKnotController(size_t knotIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorKnotControllerPrivate>(GetDataModel(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, knotIndex);
    if (result)
        m_KnotsControllers.emplace_back(result);

    return result;
}

ICurveEditorTangentControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddTangentController(size_t tangentIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorTangentControllerPrivate>(GetDataModel(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, tangentIndex);
    if (result)
        m_TangentsControllers.emplace_back(result);

    return result;
}

ICurveEditorCurveControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddCurveController(size_t curveIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorCurveControllerPrivate>(GetDataModel(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, curveIndex);
    if (result)
        m_CurvesControllers.emplace_back(result);

    return result;
}

ICurveEditorFunctionSplineControllerUniquePtr ICurveEditorFunctionSplineController::Create()
{
    return std::make_unique<CCurveEditorFunctionSplineController>();
}