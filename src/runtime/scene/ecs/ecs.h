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
    Archetype() = default;
    ~Archetype() = default;
    Archetype(ArchetypeKey key) : m_Key(key) {}

    template <typename TComponent>
    TComponent* GetData()
    {
        const auto& it = m_ComponentData.find(TComponent::ID);
        if (it == m_ComponentData.end() || it->second.empty()) {
            // TODO add editor log
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
        if (m_Entities.size() == 0) return 0;

        EntityHandle movedEntity = 0;
        size_t last = m_Entities.size() - 1;
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
            {
                data.resize(data.size() - m_ComponentSize[type]);
            }
            return movedEntity;
        }
        else if (idx == last) // Removing the entity at the end of the list
        {
            movedEntity = m_Entities[last];
            m_Entities.pop_back();
            for (auto& [type, data] : m_ComponentData)
            {
                data.resize(data.size() - m_ComponentSize[type]);
            }
            return movedEntity;
        }
    }

    bool HasComponent(U32 componentID) const
    {
        return m_ComponentSize.count(componentID) != 0;
    }

    //friend class EntityRegistry;

//TODO: Disable encapsulation for testing first
//private:
    ArchetypeKey m_Key = EmptyArchetypeKey;
    std::vector<EntityHandle> m_Entities;
    std::unordered_map<U32, std::vector<U8>> m_ComponentData;
    // data stored as binary so keep track of the component size
    // Map updated when actually adding the components from registry
    std::unordered_map<U32, size_t> m_ComponentSize;
};

class EntityRegistry
{
public:
    EntityHandle CreateEntity()
    {
        const EntityHandle e = m_NextEntityHandle;
        // Entity will start with no components
        auto& emptyArch = m_KeyToArchetype[EmptyArchetypeKey];

        m_Locations.insert({e, { EmptyArchetypeKey, emptyArch.m_Entities.size()} });
        //MoveEntity(e, EmptyArchetypeKey);
        return m_NextEntityHandle++;
    }

    template <typename TComponent>
    TComponent& GetComponent(EntityHandle e)
    {
        const auto& [key, index] = m_Locations.at(e);
        auto& arch = m_KeyToArchetype.at(key);

        TComponent* data = arch.GetData<TComponent>();
        return data[index];
    }

    template <typename TComponent>
    void AddComponent(EntityHandle e, const TComponent& value)
    {
        const auto& [oldSetKey, oldIndex] = m_Locations.at(e);
        ArchetypeKey newSetKey = oldSetKey | TComponent::ID;

        if (oldSetKey == newSetKey) {
            // TODO log (should be a editor log + console log, but not setup yet)
            // revist after creating the editor
            return; // Component to be added already exists
        }

        // Update entity with the new set key
        //m_EntityToKey[e] = newSetKey;
        //auto& oldArch = m_KeyToArchetype[oldSetKey];

        // Move data to new set
        if (m_KeyToArchetype.count(newSetKey) == 0)
        {
            m_KeyToArchetype.insert({ newSetKey, Archetype(newSetKey) });
        }

        // Move entity to new archetype set
        // while copying all data from their components except for the newly-added component
        size_t dstIndex = MoveEntity(e, newSetKey);

        // Now we prepare to copy the data for the newly-added component
        auto& dstArch = m_KeyToArchetype[newSetKey];
        dstArch.m_ComponentSize[TComponent::ID] = sizeof(TComponent);
        // Increase size of vector by one since dest Arch m_entities size should now include the new entity
        // So MoveEntity() should always be before this operation
        dstArch.m_ComponentData[TComponent::ID].resize(dstArch.m_Entities.size() * sizeof(TComponent));
        // Now copy component data to new archetype
        memcpy(&dstArch.m_ComponentData[TComponent::ID][dstIndex * sizeof(TComponent)], &value, sizeof(TComponent));
    }

    template <typename TComponent>
    void RemoveComponent(EntityHandle e)
    {
        auto& [key, index] = m_Locations.at(e);
        // Unset the bit associated with the removed component
        // Note that this depends on the enum flags in component.h using bitset values
        // reminder to self to look there if bugs appear with the Archetype set keys not being distinct
        ArchetypeKey newKey = key & ~TComponent::ID;
        if (newKey != key) {
            MoveEntity(e, newKey);
        }
    }

private: // helper funcs
    size_t MoveEntity(EntityHandle e, ArchetypeKey newKey)
    {
        auto& [oldKey, oldIndex] = m_Locations.at(e);
        Archetype& src = m_KeyToArchetype.at(oldKey);
        Archetype& dst = m_KeyToArchetype.at(newKey);

        size_t newIndex = dst.AddEntity(e);
        for (auto& [type, srcData] : src.m_ComponentData)
        {
            // Check that the type is not being removed
            // newKey will not have that bit set, so dont copy over
            // that component's data
            if (!(newKey & type)) continue;

            size_t compSize = src.m_ComponentSize.at(type);
            dst.m_ComponentSize.at(type) = compSize;
            auto& dstData = dst.m_ComponentData.at(type);
            size_t reqSize = dst.m_Entities.size() * compSize;
            if (dstData.size() < reqSize)
            {
                dstData.resize(reqSize);
            }
            // Copy data of each component to its new position in the new archetype
            memcpy(&dstData[newIndex * compSize], &srcData[oldIndex * compSize], compSize);
        }

        EntityHandle movedEnt = src.RemoveEntity(oldIndex);
        if (movedEnt) {
            m_Locations.at(movedEnt).index = oldIndex;
            m_Locations.at(movedEnt).key = newKey;
        } else {
            m_Locations.insert_or_assign(e, Location{newKey, newIndex});
        }
        return newIndex;
    }


private:
    // Entity handles are packed tightly into their corresponding vecs
    // so we need to keep track of their positions in those lists
    // for moving their data around during add/remove ops
    struct Location {
        ArchetypeKey key;
        U64 index;
    };

    std::unordered_map<EntityHandle, Location> m_Locations;
    std::unordered_map<ArchetypeKey, Archetype> m_KeyToArchetype;
    U64 m_NextEntityHandle = 1; // start at 1, will use 0 as a NULL Entity
};

} // namespace gr