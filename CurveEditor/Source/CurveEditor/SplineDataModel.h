#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_DATA_MODEL_H__)

#include <ax/ax.h>
#include "EditorDataModel.h"

using SplineID = size_t;
using SplineColor = unsigned int;

struct SSplineControlPointSinglePosition
{
    SSplineControlPointSinglePosition() = default;
    SSplineControlPointSinglePosition(size_t controlPointIndex, ax::pointf position);

    size_t ControlPointIndex;
    mutable ax::pointf Position;

    bool operator<(const SSplineControlPointSinglePosition& rhs) const noexcept;
};

using SplineControlPointsIndexes = std::set<size_t>;
using SplineControlPointsPositions = std::set<SSplineControlPointSinglePosition>;

class ICurveEditorSplineDataModelListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSplineDataModelListener() override = default;

    virtual void OnControlPointsAdded(const SplineControlPointsPositions& positions) = 0;
    virtual void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes) = 0;

    virtual void OnControlPointsModified(const SplineControlPointsPositions& positions) = 0;
};

class ICurveEditorSplineDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorSplineDataModel() override = default;

    virtual bool AddControlPoints(const SplineControlPointsPositions& positions) = 0;
    virtual bool RemoveControlPoints(const SplineControlPointsIndexes& indexes) = 0;

    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept = 0;
    virtual bool SetControlPoints(const SplineControlPointsPositions& positions) = 0;

    virtual const SplineID& GetID() const noexcept = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;

    static ICurveEditorSplineDataModelUniquePtr Create(SplineID id, SplineColor color);

    static SplineID GenerateSplineID() noexcept;
    static const SplineID& InvalidSplineID() noexcept;

    static size_t ControlPointsPerCurve() noexcept;
};

#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_H__