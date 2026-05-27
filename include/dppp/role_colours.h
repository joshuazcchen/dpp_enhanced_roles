#pragma once

#include "constants.h"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace dppp {

    struct role_colours {
        uint32_t primary_colour{0};
        std::optional<uint32_t> secondary_colour{std::nullopt};
        std::optional<uint32_t> tertiary_colour{std::nullopt};

        uint32_t &primary_color{primary_colour};
        std::optional<uint32_t> &secondary_color{secondary_colour};
        std::optional<uint32_t> &tertiary_color{tertiary_colour};

        role_colours() noexcept
            : primary_colour(0), secondary_colour(std::nullopt), tertiary_colour(std::nullopt),
            primary_color(primary_colour), secondary_color(secondary_colour), tertiary_color(tertiary_colour) {}

        explicit role_colours(uint32_t primary, std::optional<uint32_t> secondary = std::nullopt,
                std::optional<uint32_t> tertiary = std::nullopt) noexcept
            : primary_colour(primary), secondary_colour(secondary), tertiary_colour(tertiary),
            primary_color(primary_colour), secondary_color(secondary_colour), tertiary_color(tertiary_colour) {}

        role_colours(const role_colours &other) noexcept
            : primary_colour(other.primary_colour), secondary_colour(other.secondary_colour),
            tertiary_colour(other.tertiary_colour), primary_color(primary_colour), secondary_color(secondary_colour),
            tertiary_color(tertiary_colour) {}

        role_colours(role_colours &&other) noexcept
            : primary_colour(std::move(other.primary_colour)), secondary_colour(std::move(other.secondary_colour)),
            tertiary_colour(std::move(other.tertiary_colour)), primary_color(primary_colour),
            secondary_color(secondary_colour), tertiary_color(tertiary_colour) {}

        role_colours &operator=(const role_colours &other) noexcept {
            if (this != &other) {
                primary_colour = other.primary_colour;
                secondary_colour = other.secondary_colour;
                tertiary_colour = other.tertiary_colour;
            }
            return *this;
        }

        role_colours &operator=(role_colours &&other) noexcept {
            if (this != &other) {
                primary_colour = std::move(other.primary_colour);
                secondary_colour = std::move(other.secondary_colour);
                tertiary_colour = std::move(other.tertiary_colour);
            }
            return *this;
        }

        [[nodiscard]] role_style style() const noexcept {
            if (tertiary_colour.has_value()) return role_style::holographic;
            if (secondary_colour.has_value()) return role_style::gradient;
            return role_style::solid;
        }

        [[nodiscard]] bool is_solid() const noexcept { return style() == role_style::solid; }
        [[nodiscard]] bool is_gradient() const noexcept { return style() == role_style::gradient; }
        [[nodiscard]] bool is_holographic() const noexcept { return style() == role_style::holographic; }
        [[nodiscard]] bool is_enhanced() const noexcept { return !is_solid(); }
        [[nodiscard]] bool is_colourless() const noexcept { return primary_colour == 0 && !is_enhanced(); }
        [[nodiscard]] bool is_colorless() const noexcept { return is_colourless(); }

        [[nodiscard]] std::string primary_hex_colour() const { return to_hex_colour(primary_colour); }
        [[nodiscard]] std::string primary_hex_color() const { return primary_hex_colour(); }

        [[nodiscard]] std::optional<std::string> secondary_hex_colour() const {
            if (!secondary_colour) return std::nullopt;
            return to_hex_colour(*secondary_colour);
        }
        [[nodiscard]] std::optional<std::string> secondary_hex_color() const { return secondary_hex_colour(); }

        [[nodiscard]] std::optional<std::string> tertiary_hex_colour() const {
            if (!tertiary_colour) return std::nullopt;
            return to_hex_colour(*tertiary_colour);
        }
        [[nodiscard]] std::optional<std::string> tertiary_hex_color() const { return tertiary_hex_colour(); }

        [[nodiscard]] static role_colours holographic() noexcept {
            return role_colours(holographic_style_colours::primary, holographic_style_colours::secondary,
                    holographic_style_colours::tertiary);
        }

        [[nodiscard]] static role_colours gradient(uint32_t primary, uint32_t secondary) noexcept {
            return role_colours(primary, secondary, std::nullopt);
        }

        [[nodiscard]] static role_colours solid(uint32_t primary) noexcept {
            return role_colours(primary, std::nullopt, std::nullopt);
        }

        void fill_from_json(const nlohmann::json &j) {
            if (j.contains("primary_color") && !j["primary_color"].is_null()) {
                primary_colour = j["primary_color"].get<uint32_t>();
            }
            if (j.contains("secondary_color") && !j["secondary_color"].is_null()) {
                secondary_colour = j["secondary_color"].get<uint32_t>();
            }
            if (j.contains("tertiary_color") && !j["tertiary_color"].is_null()) {
                tertiary_colour = j["tertiary_color"].get<uint32_t>();
            }
        }

        [[nodiscard]] static role_colours from_role_json(const nlohmann::json &role_json) {
            role_colours rc;
            if (role_json.contains("colors") && !role_json["colors"].is_null()) {
                rc.fill_from_json(role_json["colors"]);
            } else if (role_json.contains("color") && !role_json["color"].is_null()) {
                rc.primary_colour = role_json["color"].get<uint32_t>();
            }
            return rc;
        }

        [[nodiscard]] nlohmann::json to_json() const {
            nlohmann::json j;
            j["primary_color"] = primary_colour;

            if (secondary_colour) {
                j["secondary_color"] = *secondary_colour;
            } else {
                j["secondary_color"] = nullptr;
            }

            if (tertiary_colour) {
                j["tertiary_color"] = *tertiary_colour;
            } else {
                j["tertiary_color"] = nullptr;
            }

            return j;
        }

        bool operator==(const role_colours &other) const noexcept {
            return primary_colour == other.primary_colour && secondary_colour == other.secondary_colour &&
                tertiary_colour == other.tertiary_colour;
        }

        bool operator!=(const role_colours &other) const noexcept { return !(*this == other); }
    };

    using role_colors = role_colours;

} // namespace dppp
