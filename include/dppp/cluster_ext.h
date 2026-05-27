#pragma once

#include "constants.h"
#include "enhanced_role.h"
#include "guild_tag.h"
#include "role_colours.h"

#include <dpp/dpp.h>
#include <nlohmann/json.hpp>

#include <functional>
#include <string>
#include <vector>

namespace dppp {

	template <typename T> struct result {
		T value{};
		bool success{false};
		std::string error;
		uint16_t http_status{0};
		bool feature_unavailable{false};
	};

	using enhanced_role_callback = std::function<void(const result<enhanced_role>&)>;
	using enhanced_roles_callback = std::function<void(const result<std::vector<enhanced_role>>&)>;
	using guild_tag_callback = std::function<void(const result<guild_tag>&)>;
	using confirm_callback = std::function<void(const result<bool>&)>;

	namespace detail {
		inline std::multimap<std::string, std::string> auth_headers(const dpp::cluster& bot) {
			return {
				{"Authorization", "Bot " + bot.token},
				{"Content-Type", "application/json"},
				{"User-Agent", "DiscordBot (dppp/1.0)"},
			};
		}

		inline std::string api_url(const std::string& path) { return std::string(DISCORD_API_BASE) + path; }
	} // namespace detail

	inline void get_enhanced_roles(dpp::cluster& bot, dpp::snowflake guild_id, enhanced_roles_callback callback) {
		std::string url = detail::api_url("/guilds/" + std::to_string(guild_id) + "/roles");

		bot.request(
			url, dpp::m_get,
			[guild_id, callback = std::move(callback)](const dpp::http_request_completion_t& http) {
				result<std::vector<enhanced_role>> res;
				res.http_status = static_cast<uint16_t>(http.status);

				if (http.status >= 200 && http.status < 300) {
					try {
						auto j = nlohmann::json::parse(http.body);
						if (j.is_array()) {
							for (const auto& role_json : j) {
								res.value.push_back(enhanced_role::from_json(role_json, guild_id));
							}
						}
						res.success = true;
					} catch (const std::exception& e) { res.error = std::string("JSON parse error: ") + e.what(); }
				} else {
					res.error = "HTTP " + std::to_string(http.status) + ": " + http.body;
				}
				callback(res);
			},
			"", "application/json", detail::auth_headers(bot));
	}

	inline void get_enhanced_role(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id,
								  enhanced_role_callback callback) {
		get_enhanced_roles(
			bot, guild_id,
			[role_id, callback = std::move(callback)](const result<std::vector<enhanced_role>>& list_result) {
				result<enhanced_role> res;
				res.http_status = list_result.http_status;
				res.feature_unavailable = list_result.feature_unavailable;

				if (!list_result.success) {
					res.error = list_result.error;
					callback(res);
					return;
				}

				for (const auto& role : list_result.value) {
					if (role.id == role_id) {
						res.value = role;
						res.success = true;
						callback(res);
						return;
					}
				}
				res.error = "Role " + std::to_string(role_id) + " not found in guild";
				callback(res);
			});
	}

	inline void get_guild_tag(dpp::cluster& bot, dpp::snowflake guild_id, guild_tag_callback callback) {
		std::string url = detail::api_url("/guilds/" + std::to_string(guild_id));

		bot.request(
			url, dpp::m_get,
			[guild_id, callback = std::move(callback)](const dpp::http_request_completion_t& http) {
				result<guild_tag> res;
				res.http_status = static_cast<uint16_t>(http.status);

				if (http.status >= 200 && http.status < 300) {
					try {
						auto j = nlohmann::json::parse(http.body);
						bool has_feature = false;
						if (j.contains("features") && j["features"].is_array()) {
							for (const auto& feat : j["features"]) {
								if (feat.is_string() && feat.get<std::string>() == "GUILD_TAGS") {
									has_feature = true;
									break;
								}
							}
						}
						res.feature_unavailable = !has_feature;
						res.value = guild_tag::from_guild_json(j, guild_id);
						res.success = true;
					} catch (const std::exception& e) { res.error = std::string("JSON parse error: ") + e.what(); }
				} else {
					res.error = "HTTP " + std::to_string(http.status) + ": " + http.body;
				}
				callback(res);
			},
			"", "application/json", detail::auth_headers(bot));
	}

	inline void set_role_colours(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id,
								 const role_colours& colours, confirm_callback callback = {},
								 const std::string& reason = {}) {
		std::string url = detail::api_url("/guilds/" + std::to_string(guild_id) + "/roles/" + std::to_string(role_id));

		nlohmann::json body;
		body["colors"] = colours.to_json();
		std::string body_str = body.dump();

		auto headers = detail::auth_headers(bot);
		if (!reason.empty()) headers.emplace("X-Audit-Log-Reason", reason);

		bot.request(
			url, dpp::m_patch,
			[callback = std::move(callback)](const dpp::http_request_completion_t& http) {
				if (!callback) return;
				result<bool> res;
				res.http_status = static_cast<uint16_t>(http.status);
				res.success = (http.status >= 200 && http.status < 300);
				res.value = res.success;
				if (!res.success) res.error = "HTTP " + std::to_string(http.status) + ": " + http.body;
				callback(res);
			},
			body_str, "application/json", headers);
	}

	inline void set_role_colors(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id,
								const role_colors& colors, confirm_callback callback = {},
								const std::string& reason = {}) {
		set_role_colours(bot, guild_id, role_id, colors, std::move(callback), reason);
	}

	inline void set_role_holographic(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id,
									 confirm_callback callback = {}, const std::string& reason = {}) {
		set_role_colours(bot, guild_id, role_id, role_colours::holographic(), std::move(callback), reason);
	}

	inline void set_role_gradient(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id, uint32_t primary,
								  uint32_t secondary, confirm_callback callback = {}, const std::string& reason = {}) {
		set_role_colours(bot, guild_id, role_id, role_colours::gradient(primary, secondary), std::move(callback),
						 reason);
	}

	inline void set_role_solid_colour(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id,
									  uint32_t colour, confirm_callback callback = {}, const std::string& reason = {}) {
		set_role_colours(bot, guild_id, role_id, role_colours::solid(colour), std::move(callback), reason);
	}

	inline void set_role_solid_color(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id, uint32_t color,
									 confirm_callback callback = {}, const std::string& reason = {}) {
		set_role_solid_colour(bot, guild_id, role_id, color, std::move(callback), reason);
	}

	inline guild_tag parse_guild_tag_from_event(const dpp::guild_create_t& event) {
		guild_tag gt;
		gt.guild_id = event.created.id;

		return gt;
	}

	inline enhanced_role parse_enhanced_role_from_json(const nlohmann::json& role_json, dpp::snowflake guild_id = 0) {
		return enhanced_role::from_json(role_json, guild_id);
	}

} // namespace dppp
