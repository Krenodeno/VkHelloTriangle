#ifndef VDELETER_H
#define VDELETER_H

#include <vulkan\vulkan.h>
#include <functional>

/* Declaration */
template<typename T>
class VDeleter;

template<typename T>
class VDeleter
{
public:
	VDeleter();

	VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef);

	VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef);

	VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef);

	~VDeleter();

	const T* operator &() const;

	T* replace();

	operator T() const;

	void operator=(T rhs);

	template<typename V>
	bool operator==(V rhs);

private:
	T object{ VK_NULL_HANDLE };
	std::function<void(T)> deleter;

	void cleanup();

};

#include "VDeleter.inl"

#endif // VDELETER_H