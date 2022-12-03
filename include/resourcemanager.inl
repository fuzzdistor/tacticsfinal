// resourcemanager.hpp
#include <utils.hpp>

template<typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    D(TERM_CYAN << "Loading resource with id: " << id << TERM_WHITE);
    if (m_resourceMap.find(id) != m_resourceMap.end())
        return;

    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceManager::load failed! " + filename);

    insertResource(id, std::move(resource));
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    D(TERM_CYAN << "Loading resource with id: " << id << TERM_WHITE);
    if (m_resourceMap.find(id) != m_resourceMap.end())
        return;

    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResourceManager::load failed! " + filename);

    insertResource(id, std::move(resource));
}

template<typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::unload(Identifier id)
{
    D(TERM_ORANGE << "Unloading resource with id: " << id << TERM_WHITE);
    if (auto it = m_resourceMap.find(id); it == m_resourceMap.end())
        throw std::runtime_error("ResourceManager::unload failed! ID not found in map");
    else
        m_resourceMap.erase(it);
}

template<typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
    D(TERM_GREEN << "Getting resource with id: " << id << TERM_WHITE);
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end() && "Trying to get a resource that has not been loadad yet!");

    return *found->second;
}

template<typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
    D(TERM_GREEN << "Getting resource with id: " << id << TERM_WHITE);
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end() && "Trying to get a const resource that has not been loadad yet!");

    return *found->second;
}

template<typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
