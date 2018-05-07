#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_CONTROLLER__)

#include "ax/ax.h"
#include "EditorController.h"

class ICurveEditorKnotController : public IEditorController
{
public:
    virtual ~ICurveEditorKnotController() override = default;

    virtual std::optional<ax::pointf> GetPosition() const noexcept = 0;
};

class ICurveEditorKnotControllerPrivate : public ICurveEditorKnotController
{
public:
    virtual ~ICurveEditorKnotControllerPrivate() override = default;

    virtual bool SetKnotIndex(size_t knotIndex) noexcept = 0;

    static ICurveEditorKnotControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_KNOT_CONTROLLER__