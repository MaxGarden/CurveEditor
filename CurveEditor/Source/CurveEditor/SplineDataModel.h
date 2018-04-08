#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_DATA_MODEL_H__)

#include "EditorDataModel.h"
#include "ax/ax.h"

enum class ECurveType
{
    Linear,
    Constant
};

class ICurveEditorSplineDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorSplineDataModel() override = default;

    virtual std::vector<ax::pointf>& GetControlPoints() noexcept = 0;
    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept = 0;
    virtual const std::vector<ECurveType>& GetCurvesTypes() const noexcept = 0;

    virtual const std::string& GetName() const noexcept = 0;
    virtual unsigned int GetColor() const noexcept = 0;

    static ICurveEditorSplineDataModelUniquePtr Create(std::string_view name, unsigned int color);
};

#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_H__