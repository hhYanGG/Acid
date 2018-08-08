﻿#include "Framebuffers.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderpass/Renderpass.hpp"
#include "DepthStencil.hpp"

namespace acid
{
	Framebuffers::Framebuffers(const RenderpassCreate &renderpassCreate, const Renderpass &renderPass, const Swapchain &swapchain, const DepthStencil &depthStencil, const VkExtent2D &extent, const VkSampleCountFlagBits &samples) :
		m_imageAttachments(std::vector<Texture *>()),
		m_framebuffers(std::vector<VkFramebuffer>())
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto surfaceFormat = Display::Get()->GetSurfaceFormat();

		uint32_t width = renderpassCreate.GetWidth() == 0 ? Display::Get()->GetWidth() : renderpassCreate.GetWidth();
		uint32_t height = renderpassCreate.GetHeight() == 0 ? Display::Get()->GetHeight() : renderpassCreate.GetHeight();

		for (auto &image : renderpassCreate.GetImages())
		{
			switch (image.GetType())
			{
			case ATTACHMENT_IMAGE:
				m_imageAttachments.emplace_back(new Texture(width, height, static_cast<VkFormat>(image.GetFormat()), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, samples));
				break;
			case ATTACHMENT_DEPTH:
				m_imageAttachments.emplace_back(nullptr);
				break;
			case ATTACHMENT_SWAPCHAIN:
				m_imageAttachments.emplace_back(nullptr);
				break;
			case ATTACHMENT_RESOLVE:
				m_imageAttachments.emplace_back(new Texture(width, height, surfaceFormat.format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SAMPLE_COUNT_1_BIT));
				break;
			}
		}

		m_framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments = {};

			for (auto &image : renderpassCreate.GetImages())
			{
				switch (image.GetType())
				{
				case ATTACHMENT_IMAGE:
				case ATTACHMENT_RESOLVE:
					attachments.emplace_back(GetTexture(image.GetBinding())->GetImageView());
					break;
				case ATTACHMENT_DEPTH:
					attachments.emplace_back(depthStencil.GetImageView());
					break;
				case ATTACHMENT_SWAPCHAIN:
					attachments.emplace_back(swapchain.GetImageViews().at(i));
					break;
				}
			}

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass.GetRenderpass();
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = extent.width;
			framebufferCreateInfo.height = extent.height;
			framebufferCreateInfo.layers = 1;

			Display::CheckVk(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &m_framebuffers.at(i)));
		}
	}

	Framebuffers::~Framebuffers()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto &attachment : m_imageAttachments)
		{
			delete attachment;
		}

		for (auto &framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}
