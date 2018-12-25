#include "RenderUtils.hpp"

RenderType::RenderType(RenderTypeBits bits) : flags(static_cast<unsigned int>(bits)) {

}

RenderType::RenderType(const RenderType& rhs) : flags(rhs.flags){

}

RenderType& RenderType::operator=(const RenderType& rhs) {
	flags = rhs.flags;
	return *this;
}

RenderType& RenderType::operator|=(const RenderType& rhs) {
	flags |= rhs.flags;
	return *this;
}

RenderType& RenderType::operator&=(const RenderType& rhs) {
	flags &= rhs.flags;
	return *this;
}

RenderType& RenderType::operator^=(const RenderType& rhs) {
	flags ^= rhs.flags;
	return *this;
}

RenderType RenderType::operator|(const RenderType& rhs) const {
	RenderType result(*this);
	result |= rhs;
	return result;
}

RenderType RenderType::operator&(const RenderType& rhs) const {
	RenderType result(*this);
	result &= rhs;
	return result;
}

RenderType RenderType::operator^(const RenderType& rhs) const {
	RenderType result(*this);
	result ^= rhs;
	return result;
}

bool RenderType::operator!() const {
	return !flags;
}

bool RenderType::operator==(const RenderType& rhs) const {
	return flags == rhs.flags;
}

bool RenderType::operator!=(const RenderType& rhs) const {
	return flags != rhs.flags;
}

RenderType::operator bool() const {
	return !!flags;
}

RenderType operator|(RenderTypeBits lhs, const RenderType& rhs) {
	return rhs | lhs;
}

RenderType operator&(RenderTypeBits lhs, const RenderType& rhs) {
	return rhs & lhs;
}

RenderType operator^(RenderTypeBits lhs, const RenderType& rhs) {
	return rhs ^ lhs;
}
