// resourcemanager.hpp

template<typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
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
    if (auto it = m_resourceMap.find(id); it == m_resourceMap.end())
        throw std::runtime_error("ResourceManager::unload failed! ID not found in map");
    else
        m_resourceMap.erase(it);
}

template<typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end() && "Trying to get a resource that has not been loadad yet!");

    return *found->second;
}

template<typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
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
