#ifndef HEADER_GE_VULKAN_SKYBOX_RENDERER_HPP
#define HEADER_GE_VULKAN_SKYBOX_RENDERER_HPP

#include "vulkan_wrapper.h"
#include <array>
#include <atomic>

namespace irr
{
    namespace scene { class ISceneNode; }
}

namespace GE
{
class GEVulkanArrayTexture;
class GEVulkanEnvironmentMap;

class GEVulkanSkyBoxRenderer
{
private:
    friend class GEVulkanEnvironmentMap;
    irr::scene::ISceneNode* m_skybox;

    GEVulkanArrayTexture *m_texture_cubemap, *m_diffuse_env_cubemap,
        *m_specular_env_cubemap, *m_dummy_env_cubemap;

    VkDescriptorSetLayout m_descriptor_layout, m_env_descriptor_layout;

    VkDescriptorPool m_descriptor_pool;

    VkDescriptorSet m_descriptor_set;

    std::array<VkDescriptorSet, 2> m_env_descriptor_set;

    std::atomic_bool m_skybox_loading, m_env_cubemap_loading;
public:
    // ------------------------------------------------------------------------
    GEVulkanSkyBoxRenderer();
    // ------------------------------------------------------------------------
    ~GEVulkanSkyBoxRenderer();
    // ------------------------------------------------------------------------
    void addSkyBox(irr::scene::ISceneNode* node);
    // ------------------------------------------------------------------------
    VkDescriptorSetLayout getDescriptorSetLayout() const
                                                { return m_descriptor_layout; }
    // ------------------------------------------------------------------------
    VkDescriptorSetLayout getEnvDescriptorSetLayout() const
                                            { return m_env_descriptor_layout; }
    // ------------------------------------------------------------------------
    const VkDescriptorSet* getDescriptorSet() const
    {
        if (m_skybox_loading.load() == true)
            return NULL;
        return &m_descriptor_set;
    }
    // ------------------------------------------------------------------------
    const VkDescriptorSet* getEnvDescriptorSet() const;
    // ------------------------------------------------------------------------
    void reset()
    {
        while (m_skybox_loading.load());
        while (m_env_cubemap_loading.load());
        m_skybox = NULL;
    }

};   // GEVulkanSkyBoxRenderer

}

#endif
