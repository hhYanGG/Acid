﻿#include "Buffer.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Buffer::Buffer(const VkDeviceSize &size, const VkBufferUsageFlags &usage, const VkMemoryPropertyFlags &properties) :
		m_size(size),
		m_buffer(VK_NULL_HANDLE),
		m_bufferMemory(VK_NULL_HANDLE)
	{
		if (m_size == 0)
		{
			return;
		}

		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto graphicsFamily = Display::Get()->GetGraphicsFamily();
		auto presentFamily = Display::Get()->GetPresentFamily();
		auto computeFamily = Display::Get()->GetComputeFamily();

		std::array<uint32_t, 3> queueFamily = {graphicsFamily, presentFamily, computeFamily};

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
		bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();

		Display::CheckVk(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &m_buffer));

		// Allocates buffer memory.
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		Display::CheckVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &m_bufferMemory));

		vkBindBufferMemory(logicalDevice, m_buffer, m_bufferMemory, 0);
	}

	Buffer::~Buffer()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroyBuffer(logicalDevice, m_buffer, nullptr);
		vkFreeMemory(logicalDevice, m_bufferMemory, nullptr);
	}

	uint32_t Buffer::FindMemoryType(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties)
	{
		auto physicalDevice = Display::Get()->GetPhysicalDevice();

		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			// If typefilter has a bit set to 1 and it contains the properties we indicated.
			if ((typeFilter & (1 << i)) &&
				(physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer!");
		return 0;
	}

	void Buffer::CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size)
	{
		CommandBuffer commandBuffer = CommandBuffer();

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);

		commandBuffer.End();
		commandBuffer.Submit();
	}
}
