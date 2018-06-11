#pragma once
#if !defined(__CURVE_EDITOR_SERIALIZER_TYPES_H__)

#include "ax/ax.h"
#include "tinyxml2/XMLSerialization.h"

namespace xmls
{
    class xFloat final : public MemberBase
    {
    public:
        xFloat() = default;
        xFloat(float value);
        virtual ~xFloat() override final = default;

        float value() const;
        xFloat& operator=(float rhs);

    private:
        void AssignValue(float value);
    };

    class xSizeT final : public MemberBase
    {
    public:
        xSizeT() = default;
        xSizeT(size_t value);
        virtual ~xSizeT() override final = default;

        size_t value() const;
        xSizeT& operator=(size_t rhs);

    private:
        void AssignValue(size_t value);
    };
}

class CCurveEditorPointfSeriaizable final : public xmls::Serializable
{
public:
    CCurveEditorPointfSeriaizable();
    CCurveEditorPointfSeriaizable(const ax::pointf& value);
    virtual ~CCurveEditorPointfSeriaizable() override final = default;

    void SetValue(const ax::pointf& value);
    ax::pointf GetValue() const;

private:
    void Initialize();

private:
    xmls::xFloat m_X;
    xmls::xFloat m_Y;
};

class CCurveEditorStringSerializable final : public xmls::Serializable
{
public:
    CCurveEditorStringSerializable();
    CCurveEditorStringSerializable(std::string&& value);
    virtual ~CCurveEditorStringSerializable() override final = default;

    void SetValue(std::string&& value);
    const std::string& GetValue() const noexcept;

private:
    void Initialize();

private:
    xmls::xString m_Value;
};


#endif //__CURVE_EDITOR_SERIALIZER_TYPES_H__