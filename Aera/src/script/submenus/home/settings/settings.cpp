#include "settings.h"
#include "command.h"
#include "thread pool/threadpool.h"
#include "util/imageLoaderHelpers.h"

namespace ui::submenus
{
	void settingsSubmenu::init(submenu& submenu)
	{
		submenu.add(submenuOption(settings::commandSubmenu::get()));
		submenu.add(submenuOption(headerSubmenu::get()));
		submenu.add(submenuOption(subtitleSubmenu::get()));
		submenu.add(submenuOption(optionsSubmenu::get()));
		submenu.add(submenuOption(footerSubmenu::get()));
		submenu.add(submenuOption(descriptionSubmenu::get()));
		submenu.add(breakOption("Width"));
		submenu.add(numberOption("Menu Width", "Change menu width", g_width, 0.01f, 0.5f, 0.005f, true));
		submenu.add(breakOption("Position"));
		submenu.add(numberOption("Menu Pos X", "Change menu pos X", g_pos.x, 0.f, 1.f, 0.005f, true));
		submenu.add(numberOption("Menu Pos Y", "Change menu pos Y", g_pos.y, 0.f, 0.9f, 0.005f, true));
		submenu.add(breakOption("Scroll"));
		submenu.add(numberOption("Scroll Speed", "Change menu scroll speed", g_options.m_scrollSpeed, 1.f, 100.f, 1.f,
		                         true));
		submenu.add(option("forceQuitToSp"_AC));
		submenu.add(option("restartGame"_AC));
		submenu.add(option("unload"_AC));
		submenu.add(option("exit"_AC));
	}

	// Header
	void headerSubmenu::init(submenu& submenu)
	{
		submenu.add(option("Load Default Header", []
		{
			if (image_loader::m_has_header_loaded && !image_loader::texture_loading_in_progress)
			{
				image_loader::m_has_header_loaded = false;
				image_loader::m_header.clear();
				image_loader::m_header_frame = 0;
			}
		}));
		if (fs::path header_path{std::getenv("appdata")}; exists(header_path.append(BRAND).append("Headers")) && !
			header_path.empty())
		{
			//submenu.add(breakOption("Custom Headers"));
			for (const auto& dir_entry : fs::directory_iterator(header_path))
			{
				if (dir_entry.is_regular_file() && image_loader::contain_pict_ext(dir_entry.path()))
				{
					submenu.add(option(
						std::format("Load {}", dir_entry.path().filename().string()),
						[path = dir_entry.path()]
						{
							if (!exists(path))
							{
								return;
							}

							if (image_loader::texture_loading_in_progress)
							{
								return;
							}

							image_loader::texture_loading_in_progress = true;
							g_thread_pool->push([path]
							{
								image_loader::m_has_header_loaded = false;
								image_loader::m_header.clear();
								image_loader::m_header_frame = 0;

								if (path.extension() == ".gif")
								{
									image_loader::m_header = image_loader::create_gif_texture(path);
								}
								else
								{
									image_loader::m_header.try_emplace(0, 0, image_loader::create_texture(path));
								}

								image_loader::m_has_header_loaded = true;
								image_loader::texture_loading_in_progress = false;
							});
						}
					));
				}
				else
				{
					submenu.add(option(
						std::format("{} invalid extension format", dir_entry.path().filename().string()),
						"Make sure file contains any of the following extensions: .png .jpeg .bmp .psd .tga .gif .hdr .pic .ppm .pgm"
					));
				}
			}
		}
		else
		{
			create_directory(header_path);
			submenu.add(option(
				"Headers folder has been successfully created",
				"The headers folder was either not located or had not been generated; it has now been established, "
				"allowing for the placement of header images within the designated directory."
			));
		}
	}

	// Subtitle
	void subtitleSubmenu::init(submenu& submenu)
	{
		submenu.add(toggleOption("Enable", g_subtitle.m_enabled));
		submenu.add(breakOption("Customizations"));
		submenu.add(numberOption("Size", "Change subtitle size", g_subtitle.m_size, 0.01f, 0.05f, 0.01f));
		submenu.add(numberOption("Padding", "Change subtitle Padding", g_subtitle.m_padding, 1.5f, 2.5f, 0.1f));
		submenu.add(breakOption("Colors"));
		submenu.add(numberOption("Red", "Change subtitle color red", g_subtitle.m_color.r, 0,
		                         255, 1, true));
		submenu.add(numberOption("Green", "Change subtitle color green", g_subtitle.m_color.g, 0,
		                         255, 1, true));
		submenu.add(numberOption("Blue", "Change subtitle color blue", g_subtitle.m_color.b, 0,
		                         255, 1, true));
		submenu.add(breakOption("Font"));
		submenu.add(numberOption("Font", "Change subtitle font", g_subtitle.m_font, 1, 6, 1, true));
	}

	// Options
	void optionsSubmenu::init(submenu& submenu)
	{
		submenu.add(numberOption("Size", "Change option size", g_options.m_size, 0.01f, 0.05f, 0.005f));
		submenu.add(numberOption("Padding", "Change option Padding", g_options.m_padding, 1.5f, 2.5f, 0.1f));
		submenu.add(numberOption("Text Size", "Change option text size", g_options.m_textSize, 0.1f, 0.4f, 0.01f));
		//submenu.add(breakOption("Colors"));
		submenu.add(numberOption("Red", "Change option color red", g_options.m_color.r, 0,
		                         255, 1, true));
		submenu.add(numberOption("Green", "Change option color green", g_options.m_color.g, 0,
		                         255, 1, true));
		submenu.add(numberOption("Blue", "Change option color blue", g_options.m_color.b, 0,
		                         255, 1, true));
		//submenu.add(breakOption("Font"));
		submenu.add(numberOption("Font", "Change option font", g_options.m_font, 1, 6, 1, true));
	}

	// Footer
	void footerSubmenu::init(submenu& submenu)
	{
		submenu.add(toggleOption("Enable", g_footer.m_enabled));
		submenu.add(breakOption("Customizations"));
		submenu.add(numberOption("Size", "Change footer size", g_footer.m_size, 0.01f, 0.05f, 0.01f));
		submenu.add(breakOption("Colors"));
		submenu.add(numberOption("Red", "Change footer color red", g_footer.m_color.r, 0, 255, 1, true));
		submenu.add(numberOption("Green", "Change footer color green", g_footer.m_color.g, 0, 255, 1, true));
		submenu.add(numberOption("Blue", "Change footer color blue", g_footer.m_color.b, 0, 255, 1, true));
	}

	// Description
	void descriptionSubmenu::init(submenu& submenu)
	{
		submenu.add(toggleOption("Enable", g_description.m_enabled));
		submenu.add(breakOption("Customizations"));
		submenu.add(numberOption("Size", "Change description size", g_description.m_size, 0.01f, 0.05f, 0.01f));
		submenu.add(numberOption("Padding", "Change description Padding", g_description.m_padding, 1.5f, 2.5f, 0.1f));
		submenu.add(breakOption("Colors"));
		submenu.add(numberOption("Red", "Change description color red", g_description.m_color.r, 0, 255, 1, true));
		submenu.add(numberOption("Green", "Change description color green", g_description.m_color.g, 0, 255, 1,
		                         true));
		submenu.add(numberOption("Blue", "Change description color blue", g_description.m_color.b, 0, 255, 1,
		                         true));
		submenu.add(breakOption("Font"));
		submenu.add(numberOption("Font", "Change description font", g_description.m_font, 1, 6, 1, true));
	}
}
