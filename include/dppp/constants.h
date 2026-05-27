#pragma once

#include <cstdint>
#include <string>

namespace dppp {

static constexpr const char *DISCORD_API_VERSION = "10";
static constexpr const char *DISCORD_API_BASE = "https://discord.com/api/v10";
static constexpr const char *DISCORD_CDN_BASE = "https://cdn.discordapp.com";

struct holographic_style_colours {
  static constexpr uint32_t primary = 11127295u;
  static constexpr uint32_t secondary = 16759788u;
  static constexpr uint32_t tertiary = 16761760u;
};

using holographic_style_colors = holographic_style_colours;

enum class role_style : uint8_t {
  solid = 0,
  gradient = 1,
  holographic = 2,
};

enum class image_format : uint8_t {
  png = 0,
  jpg = 1,
  webp = 2,
  gif = 3,
  auto_detect = 4,
};

[[nodiscard]] inline std::string format_extension(image_format fmt) {
  switch (fmt) {
  case image_format::jpg:
    return "jpg";
  case image_format::webp:
    return "webp";
  case image_format::gif:
    return "gif";
  case image_format::png:
  case image_format::auto_detect:
  default:
    return "png";
  }
}

[[nodiscard]] inline std::string to_hex_colour(uint32_t colour) {
  char buf[8];
  std::snprintf(buf, sizeof(buf), "#%06X", colour & 0x00FFFFFFu);
  return std::string(buf);
}

[[nodiscard]] inline std::string to_hex_color(uint32_t color) {
  return to_hex_colour(color);
}

[[nodiscard]] inline uint8_t red(uint32_t colour) noexcept {
  return static_cast<uint8_t>((colour >> 16u) & 0xFFu);
}

[[nodiscard]] inline uint8_t green(uint32_t colour) noexcept {
  return static_cast<uint8_t>((colour >> 8u) & 0xFFu);
}

[[nodiscard]] inline uint8_t blue(uint32_t colour) noexcept {
  return static_cast<uint8_t>(colour & 0xFFu);
}

[[nodiscard]] inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) noexcept {
  return (static_cast<uint32_t>(r) << 16u) | (static_cast<uint32_t>(g) << 8u) |
         static_cast<uint32_t>(b);
}

} // namespace dppp
