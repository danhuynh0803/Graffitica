#pragma once

#include <vector>
#include <unordered_map>
#include "core/types.h"
#include "component.h"

namespace gr
{

using EntityHandle = U64;
constexpr EntityHandle NullEntity = 0;

// Just a bitmask holding all assigned components
using ArchetypeKey = U32;
constexpr ArchetypeKey EmptyArchetypeKey = 0;

// Every combination of components will be represented with an archetype
class Archetype
{
public:
    ArchetypeKey m_Key = EmptyArchetypeKey;


    template <typename TComponent>
    TComponent* GetData()
    {
        const auto& it = m_ComponentData.find(TComponent::ID);
        if (it == m_ComponentData.end() || it->second.empty()) {
            // TODO add log
            return nullptr; // component doesnt exist
        }
        return reinterpret_cast<TComponent*>(it->second.data());
    }
    
    size_t AddEntity(EntityHandle e)
    {
        const size_t idx = m_Entities.size();
        m_Entities.push_back(e);
        for (auto& [id, binary] : m_ComponentData)
        {
            // Grow all binary byte buffers by one, since size result of binary vector is byte-length
            binary.resize(binary.size() + m_ComponentSize[id]);
        }
        return idx;
    }

    // Removes an entity given the index/handle
    // However, we also need to fill the remove entity to avoid the entity list from turning sparse.
    // This can be achieved by moving the last element to the recently-removed slot.
    EntityHandle RemoveEntity(size_t idx)
    {
        size_t last = m_Entities.size() - 1;
        EntityHandle movedEntity = 0;
        if (idx != last)
        {
            movedEntity = m_Entities[last];
            m_Entities[idx] = movedEntity;
            // Copy the last entry in the last to the idx that is getting removed
            for (auto& [type, data] : m_ComponentData)
            {
                memcpy(&data[idx * m_ComponentSize[type]],
                       &data[last * m_ComponentSize[type]],
                       m_ComponentSize[type]);
            }
            // Remove last element post-copy operation
            m_Entities.pop_back();
            for (auto& [type, data] : m_ComponentData)
                data.resize(data.size() - m_ComponentSize[type]);

            return movedEntity;
        }
    }

    bool HasComponent(U32 componentID) const
    {
        return m_ComponentSize.count(componentID) != 0;
    }

private:
    std::vector<EntityHandle> m_Entities;
    std::unordered_map<U32, std::vector<U8>> m_ComponentData;
    // data stored as binary so keep track of the component size
    // Map updated when actually adding the components from registry
    std::unordered_map<U32, size_t> m_ComponentSize;
};

class EntityRegistry
{
public:
    EntityHandle CreateEntity();

    template <typename TComponent>
    void AddComponent(EntityHandle e, const TComponent& comp)
    {
        const ArchetypeKey oldSetKey = m_EntityToKey[e];
        ArchetypeKey newSetKey = oldSetKey | TComponent::ID;

        if (oldSetKey == newSetKey) {
            // TODO log (should be a editor log + console log, but not setup yet)
            // revist after creating the editor
            return; // Component to be added already exists
        }

        // Update entity with the new set key
        m_EntityToKey[e] = newSetKey;
        auto& set = m_KeyToArchetype[oldSetKey];

        // Move data to new set
    }

    template <typename TComponent>
    void RemoveComponent(EntityHandle e)
    {

    }

private:
    std::unordered_map<EntityHandle, ArchetypeKey> m_EntityToKey;
    std::unordered_map<EntityHandle, size_t> m_EntityToRow;
    std::unordered_map<ArchetypeKey, Archetype> m_KeyToArchetype;
    U64 m_NextEntityHandle = 1; // start at 1, will use 0 as a NULL Entity
};

} // namespace gr