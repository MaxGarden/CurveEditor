#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_DATA_MODEL_H__)

#include "EditorDataModel.h"
#include "ax/ax.h"

enum class ECurveType
{
    Linear,
    Constant
};

class CCurveEditorSplineDataModel final : public IEditorDataModel
{
public:
    CCurveEditorSplineDataModel() = default;
    virtual ~CCurveEditorSplineDataModel() override final = default;



private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurveTypes;
};


#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_H__