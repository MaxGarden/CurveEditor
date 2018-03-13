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
    CCurveEditorSplineDataModel(std::string&& name);
    virtual ~CCurveEditorSplineDataModel() override final = default;

    std::vector<ax::pointf>& GetControlPoints() noexcept;
	const std::string& GetName() const noexcept;

private:
    std::vector<ax::pointf> m_ControlPoints;
    std::vector<ECurveType> m_CurveTypes;
	std::string m_Name;
};


#endif //__CURVE_EDITOR_SPLINE_DATA_MODEL_H__