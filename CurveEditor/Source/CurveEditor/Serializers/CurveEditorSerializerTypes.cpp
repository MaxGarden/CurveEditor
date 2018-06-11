#include "pch.h"
#include "CurveEditorSerializerTypes.h"
#include <sstream>

xmls::xFloat::xFloat(float value)
{
    AssignValue(value);
}

float xmls::xFloat::value() const
{
    std::stringstream stringStream{ m_sValue };
    float value;

    const auto streamResult = !(stringStream >> value).fail();
    EDITOR_ASSERT(streamResult);
    if(!streamResult)
        return 0.0f;

    return value;
}

xmls::xFloat& xmls::xFloat::operator=(float rhs)
{
    AssignValue(rhs);
    return *this;
}

void xmls::xFloat::AssignValue(float value)
{
    std::stringstream stringStream;
    stringStream << value;
    m_sValue = stringStream.str();
}

CCurveEditorPointfSeriaizable::CCurveEditorPointfSeriaizable()
{
    Initialize();
}

CCurveEditorPointfSeriaizable::CCurveEditorPointfSeriaizable(const ax::pointf& value)
{
    Initialize();
    SetValue(value);
}

void CCurveEditorPointfSeriaizable::SetValue(const ax::pointf& value)
{
    m_X = value.x;
    m_Y = value.y;
}

xmls::xSizeT::xSizeT(size_t value)
{
    AssignValue(value);
}

size_t xmls::xSizeT::value() const
{
    std::stringstream stringStream{ m_sValue };
    size_t value;

    const auto streamResult = !(stringStream >> value).fail();
    EDITOR_ASSERT(streamResult);
    if (!streamResult)
        return 0u;

    return value;
}

xmls::xSizeT& xmls::xSizeT::operator=(size_t rhs)
{
    AssignValue(rhs);
    return *this;
}

void xmls::xSizeT::AssignValue(size_t value)
{
    std::stringstream stringStream;
    stringStream << value;
    m_sValue = stringStream.str();
}

ax::pointf CCurveEditorPointfSeriaizable::GetValue() const
{
    return { m_X.value(), m_Y.value() };
}

void CCurveEditorPointfSeriaizable::Initialize()
{
    setClassName("Point");
    Register("x", &m_X);
    Register("y", &m_Y);
}

CCurveEditorStringSerializable::CCurveEditorStringSerializable()
{
    Initialize();
}

CCurveEditorStringSerializable::CCurveEditorStringSerializable(std::string&& value)
{
    Initialize();
    SetValue(std::move(value));
}

void CCurveEditorStringSerializable::SetValue(std::string&& value)
{
    m_Value = std::move(value);
}

const std::string& CCurveEditorStringSerializable::GetValue() const noexcept
{
    return m_Value.value();
}

void CCurveEditorStringSerializable::Initialize()
{
    setClassName("StringData");
    Register("Data", &m_Value);
}