#pragma once

#include "constants.h"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace dppp {

	struct guild_tag {
		uint64_t guild_id{0};
		std::string tag;
		std::string badge_hash;

		[[nodiscard]] bool has_tag() const noexcept { return !tag.empty(); }
		[[nodiscard]] bool has_badge() const noexcept { return !badge_hash.empty(); }

		[[nodiscard]] std::string badge_url(image_format fmt = image_format::png, uint16_t size = 0) const {
			if (badge_hash.empty() || guild_id == 0) return {};
			std::string url = std::string(DISCORD_CDN_BASE) + "/guild-tag-icons/" + std::to_string(guild_id) + "/" +
							  badge_hash + "." + format_extension(fmt);
			if (size > 0) url += "?size=" + std::to_string(size);
			return url;
		}

		void fill_from_json(const nlohmann::json& profile_json) {
			if (profile_json.contains("tag") && !profile_json["tag"].is_null()) {
				tag = profile_json["tag"].get<std::string>();
			}
			if (profile_json.contains("badge") && !profile_json["badge"].is_null()) {
				badge_hash = profile_json["badge"].get<std::string>();
			}
		}

		[[nodiscard]] static guild_tag from_guild_json(const nlohmann::json& guild_json, uint64_t gid = 0) {
			guild_tag gt;
			gt.guild_id = gid;
			if (guild_json.contains("profile") && !guild_json["profile"].is_null()) {
				gt.fill_from_json(guild_json["profile"]);
			}
			return gt;
		}
	};

} // namespace dppp
