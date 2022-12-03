#ifndef TF_RESOURCEMANAGER_HPP
#define TF_RESOURCEMANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <utility>

namespace sf
{
    class Texture;
    class Font;
}

template <typename Resource, typename Identifier>
class ResourceManager
{
    friend struct ResourcePack;

public:
    void load(Identifier id, const std::string& filename);

    template<typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);

    void unload(Identifier id);

    [[nodiscard]] Resource& get(Identifier id);
    [[nodiscard]] const Resource& get(Identifier id) const;

protected:
    // default protected constructor para evitar instanciar fuera de ResourcePack
    // deleted copy constructors para evitar copias
    ResourceManager() = default;
    ResourceManager(const ResourceManager<Resource, Identifier>&) = delete;
    ResourceManager& operator=(const ResourceManager<Resource, Identifier>&) = delete;

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);


    std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

struct ResourcePack
{
    static ResourcePack& getInstance()
    {
        static ResourcePack pack;
        return pack;
    }

    ResourceManager<sf::Texture, size_t> textures;
    ResourceManager<sf::Font, size_t> fonts;


protected:
    // default protected constructor para evitar instanciar fuera del getInstance
    ResourcePack() = default;
    // deleted copy constructors para evitar copias
    ResourcePack(const ResourcePack&) = delete;
    ResourcePack& operator=(const ResourcePack&) = delete;
};

#include "resourcemanager.inl"
#endif // TF_RESOURCEMANAGER_HPP

