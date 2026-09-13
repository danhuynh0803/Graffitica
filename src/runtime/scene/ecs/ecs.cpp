#include "ecs.h"

namespace gr
{

EntityHandle gr::EntityRegistry::CreateEntity()
{
    // Entity will start with no components
    m_EntityToKey[m_NextEntityHandle] = ArchetypeKey{0};
    return m_NextEntityHandle++;
}


} //namespace gr