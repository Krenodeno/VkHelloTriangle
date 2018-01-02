template <typename T>
VDeleter<T>::VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {})
{}

template<typename T>
VDeleter<T>::VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [=](T obj) { deletef(obj, nullptr); };
}

template<typename T>
VDeleter<T>::VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
}

template<typename T>
VDeleter<T>::VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
{
	this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
}

template<typename T>
VDeleter<T>::~VDeleter() {
	cleanup();
}

template<typename T>
const T* VDeleter<T>::operator &() const {
	return &object;
}

template<typename T>
T* VDeleter<T>::replace() {
	cleanup();
	return &object;
}

template<typename T>
VDeleter<T>::operator T() const {
	return object;
}

template<typename T>
void VDeleter<T>::operator=(T rhs) {
	if (rhs != object) {
		cleanup();
		object = rhs;
	}
}

template<typename T>
template<typename V>
bool VDeleter<T>::operator==(V rhs) {
	return object == T(rhs);
}

template<typename T>
void VDeleter<T>::cleanup() {
	if (object != VK_NULL_HANDLE) {
		deleter(object);
	}
	object = VK_NULL_HANDLE;
}