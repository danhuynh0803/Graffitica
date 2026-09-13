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
    TComponent* GetComponentData()
    {
        const auto& it = m_IDToComponentDataBinary.find(TComponent::ID);
        if (it == m_IDToComponentDataBinary.end() || it->second.empty())
            return nullptr;
        return static_cast<TComponent*>(it->second.data());
    }

    bool HasComponent(U32 componentID) const
    {
        return m_ComponentTypeToSizeMap.count(componentID) != 0;
    }

private:
    std::vector<EntityHandle> m_Entities;
    std::unordered_map<U32, std::vector<U8>> m_IDToComponentDataBinary;
    // data stored as binary so keep track of the component size
    // Map updated when actually adding the components from registry
    std::unordered_map<U32, size_t> m_ComponentTypeToSizeMap;
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

        if (oldSetKey == newSetKey) return; // Component to be added already exists

        // Update entity with the new set key
        m_EntityToKey[e] = newSetKey;
        auto& set = m_KeyToArchetype[oldSetKey];

        // Move data to new set
    }

private:
    std::unordered_map<EntityHandle, ArchetypeKey> m_EntityToKey;
    std::unordered_map<EntityHandle, size_t> m_EntityToRow;
    std::unordered_map<ArchetypeKey, Archetype> m_KeyToArchetype;
    U64 m_NextEntityHandle = 1; // start at 1, will use 0 as a NULL Entity
};

} // namespace gr