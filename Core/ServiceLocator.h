#ifndef DENDY_TANKS_SERVICE_LOCATOR_H
#define DENDY_TANKS_SERVICE_LOCATOR_H

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>

/**
 * @class ServiceLocator
 * @brief Provides a service locator pattern implementation for dependency injection
 * 
 * This class allows components to access services without direct dependencies,
 * promoting loose coupling between components.
 */
class ServiceLocator {
public:
    /**
     * @brief Get the singleton instance of the ServiceLocator
     * @return Reference to the ServiceLocator instance
     */
    static ServiceLocator& getInstance() {
        static ServiceLocator instance;
        return instance;
    }
    
    /**
     * @brief Register a service
     * @tparam T The service type
     * @tparam U The concrete implementation type
     * @param service Pointer to the service instance
     */
    template<typename T, typename U = T>
    void registerService(U* service) {
        if (!service) {
            throw std::invalid_argument("Cannot register null service");
        }
        
        // Store the service using type_index as key
        services_[std::type_index(typeid(T))] = static_cast<void*>(service);
        
        // Store the ownership flag
        ownedServices_[std::type_index(typeid(T))] = false;
    }
    
    /**
     * @brief Register a service with ownership
     * @tparam T The service type
     * @tparam U The concrete implementation type
     * @param service Unique pointer to the service instance
     */
    template<typename T, typename U = T>
    void registerService(std::unique_ptr<U> service) {
        if (!service) {
            throw std::invalid_argument("Cannot register null service");
        }
        
        // Store the service using type_index as key
        services_[std::type_index(typeid(T))] = static_cast<void*>(service.release());
        
        // Store the ownership flag
        ownedServices_[std::type_index(typeid(T))] = true;
    }
    
    /**
     * @brief Get a service
     * @tparam T The service type
     * @return Pointer to the service instance
     * @throws std::runtime_error if service is not registered
     */
    template<typename T>
    T* getService() const {
        auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end()) {
            throw std::runtime_error("Service not registered: " + std::string(typeid(T).name()));
        }
        
        return static_cast<T*>(it->second);
    }
    
    /**
     * @brief Check if a service is registered
     * @tparam T The service type
     * @return True if the service is registered, false otherwise
     */
    template<typename T>
    bool hasService() const {
        return services_.find(std::type_index(typeid(T))) != services_.end();
    }
    
    /**
     * @brief Unregister a service
     * @tparam T The service type
     */
    template<typename T>
    void unregisterService() {
        auto it = services_.find(std::type_index(typeid(T)));
        if (it != services_.end()) {
            // Check if we own this service
            auto ownIt = ownedServices_.find(std::type_index(typeid(T)));
            if (ownIt != ownedServices_.end() && ownIt->second) {
                // We own it, so delete it
                delete static_cast<T*>(it->second);
            }
            
            // Remove from maps
            services_.erase(it);
            ownedServices_.erase(std::type_index(typeid(T)));
        }
    }
    
    /**
     * @brief Clear all services
     */
    void clear() {
        // Delete all owned services
        for (const auto& [type, owned] : ownedServices_) {
            if (owned) {
                delete static_cast<void*>(services_[type]);
            }
        }
        
        // Clear maps
        services_.clear();
        ownedServices_.clear();
    }
    
    /**
     * @brief Destructor
     */
    ~ServiceLocator() {
        clear();
    }

private:
    // Private constructor for singleton pattern
    ServiceLocator() = default;
    
    // Prevent copying and assignment
    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator=(const ServiceLocator&) = delete;
    
    // Service storage
    std::unordered_map<std::type_index, void*> services_;
    
    // Ownership flags
    std::unordered_map<std::type_index, bool> ownedServices_;
};

#endif // DENDY_TANKS_SERVICE_LOCATOR_H