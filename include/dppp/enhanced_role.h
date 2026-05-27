#pragma once

#include "constants.h"
#include "role_colours.h"
#include <cstdint>
#include <dpp/snowflake.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace dpp {
	class role;
}

namespace dppp {

	struct enhanced_role {
		dpp::snowflake id{0};
		std::string name;
		dpp::snowflake guild_id{0};
		int32_t position{0};

		role_colours colours;
		role_colours& colors{colours};

		std::string icon_hash;
		std::string unicode_emoji;

		enhanced_role() noexcept : colours(), colors(colours) {}

		enhanced_role(const enhanced_role& other)
			: id(other.id), name(other.name), guild_id(other.guild_id), position(other.position),
			  colours(other.colours), colors(colours), icon_hash(other.icon_hash), unicode_emoji(other.unicode_emoji) {}

		enhanced_role(enhanced_role&& other) noexcept
			: id(std::move(other.id)), name(std::move(other.name)), guild_id(std::move(other.guild_id)),
			  position(std::move(other.position)), colours(std::move(other.colours)), colors(colours),
			  icon_hash(std::move(other.icon_hash)), unicode_emoji(std::move(other.unicode_emoji)) {}

		enhanced_role& operator=(const enhanced_role& other) {
			if (this != &other) {
				id = other.id;
				name = other.name;
				guild_id = other.guild_id;
				position = other.position;
				colours = other.colours;
				icon_hash = other.icon_hash;
				unicode_emoji = other.unicode_emoji;
			}
			return *this;
		}

		enhanced_role& operator=(enhanced_role&& other) noexcept {
			if (this != &other) {
				id = std::move(other.id);
				name = std::move(other.name);
				guild_id = std::move(other.guild_id);
				position = std::move(other.position);
				colours = std::move(other.colours);
				icon_hash = std::move(other.icon_hash);
				unicode_emoji = std::move(other.unicode_emoji);
			}
			return *this;
		}

		[[nodiscard]] bool has_icon() const noexcept { return !icon_hash.empty(); }
		[[nodiscard]] bool has_unicode_emoji() const noexcept { return !unicode_emoji.empty(); }

		[[nodiscard]] std::string icon_url(image_format fmt = image_format::png, uint16_t size = 0) const {
			if (icon_hash.empty() || id == 0) return {};
			std::string url = std::string(DISCORD_CDN_BASE) + "/role-icons/" + std::to_string(id) + "/" + icon_hash +
							  "." + format_extension(fmt);
			if (size > 0) { url += "?size=" + std::to_string(size); }
			return url;
		}

		[[nodiscard]] bool has_enhanced_colour() const noexcept { return colours.is_enhanced(); }
		[[nodiscard]] bool has_enhanced_color() const noexcept { return has_enhanced_colour(); }

		[[nodiscard]] role_style colour_style() const noexcept { return colours.style(); }
		[[nodiscard]] role_style color_style() const noexcept { return colour_style(); }

		void fill_from_json(const nlohmann::json& j, dpp::snowflake gid = 0) {
			guild_id = gid;
			if (j.contains("id") && !j["id"].is_null()) {
				auto id_val = j["id"];
				if (id_val.is_string()) {
					id = static_cast<dpp::snowflake>(std::stoull(id_val.get<std::string>()));
				} else {
					id = id_val.get<dpp::snowflake>();
				}
			}
			if (j.contains("name") && !j["name"].is_null()) name = j["name"].get<std::string>();
			if (j.contains("position") && !j["position"].is_null()) position = j["position"].get<int32_t>();

			colours = role_colours::from_role_json(j);

			if (j.contains("icon") && !j["icon"].is_null()) icon_hash = j["icon"].get<std::string>();
			if (j.contains("unicode_emoji") && !j["unicode_emoji"].is_null()) {
				unicode_emoji = j["unicode_emoji"].get<std::string>();
			}
		}

		[[nodiscard]] static enhanced_role from_json(const nlohmann::json& j, dpp::snowflake gid = 0) {
			enhanced_role er;
			er.fill_from_json(j, gid);
			return er;
		}

		[[nodiscard]] nlohmann::json colours_to_json() const {
			nlohmann::json j;
			j["colors"] = colours.to_json();
			return j;
		}

		[[nodiscard]] nlohmann::json colors_to_json() const { return colours_to_json(); }
	};

} // namespace dppp
