#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_DATA_MODEL_H__)

#include <ax/ax.h>
#include "EditorDataModel.h"

enum class ECurveType
{
    Linear,
    Constant
};

using SplineID = size_t;
using SplineColor = unsigned int;

class ICurveEditorSplineDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorSplineDataModel() override = default;

    virtual std::vector<ax::pointf>& GetControlPoints() noexcept = 0;
    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept = 0;
    virtual const std::vector<ECurveType>& GetCurvesTypes() const noexcept = 0;

    virtual const SplineID& GetID() const noexcept = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;

    static ICurveEditorSplineDataModelUniquePtr Create(SplineID id, SplineColor color);

    static SplineID GenerateSplineID() noexcept;
    static const SplineID& InvalidSplineID() noexcept;
};

#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_H__