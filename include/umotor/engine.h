#pragma once

#include <memory>
#include <string>
#include <cassert>
#include <functional>
#include <map>
#include <typeindex>
#include <any>

namespace umotor {
    
class Scene;
class Engine {
public:
    virtual ~Engine() = default;
    void run();
    void stop();

    template <typename T, typename... Args>
    T& switch_scene(Args&&... args) {
        std::unique_ptr<T> t = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *t;
        if (is_running)
            next_scene = std::move(t);
        else
            current_scene = std::move(t);
        return ref;
    }

    void switch_scene(std::unique_ptr<Scene> scene);

    
    template <typename AssetType, typename AssetKey>
    void register_asset_manager(std::function<std::unique_ptr<AssetType>(AssetKey key)> load_function) {
        std::pair<std::type_index, std::type_index> lookup_key = { std::type_index(typeid(AssetType)), std::type_index(typeid(AssetKey)) };
        
        std::unique_ptr<AssetManager<AssetType, AssetKey>> manager = std::make_unique<AssetManager<AssetType, AssetKey>>();
        manager->load_function = std::move(load_function);
        asset_managers[lookup_key] = std::move(manager);
    }

    template <typename AssetType, typename AssetKey>
    AssetType* get_asset(AssetKey key) {
        std::pair<std::type_index, std::type_index> lookup_key = { std::type_index(typeid(AssetType)), std::type_index(typeid(AssetKey)) };

        AssetManager<AssetType, AssetKey>& manager = dynamic_cast<AssetManager<AssetType, AssetKey>&>(*asset_managers.at(lookup_key));
        std::unique_ptr<AssetType>& entry = manager.cache[key];
        if (!entry) entry = manager.load_function(key); 
        return entry.get();
    }

protected:
    virtual void open_window() = 0;
    virtual float get_delta() = 0;
    virtual bool should_stop() = 0;
    virtual void begin_rending() = 0;
    virtual void end_rendering() = 0;
    
    virtual void handle_events();
private:
    struct IAssetManager{
        virtual ~IAssetManager() = default;
    };

    template <typename AssetType, typename AssetKey>
    struct AssetManager : public IAssetManager {
        std::map<AssetKey, std::unique_ptr<AssetType>> cache;
        std::function<std::unique_ptr<AssetType>(AssetKey key)> load_function;
    };

    void end_of_frame();

    std::unique_ptr<Scene> current_scene;
    std::unique_ptr<Scene> next_scene;

    std::map<std::pair<std::type_index, std::type_index>, std::unique_ptr<IAssetManager>> asset_managers;

    bool is_running = false;
};

};