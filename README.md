# dpp_enhanced_roles

A small add-on for the [D++](https://github.com/brainboxdotcc/DPP/) Discord library to allow compatibility with Discord's enhanced roles features.

-------

## Features

- Server tag support
- Gradient role support
- Holographic role support

I am almost certain there was a way to use these features natively, but unlike discord.js, D++ didn't have a native &->colours option, and I did not directly spot anything regarding gradients in its documentation.

-------

## Example Usage

This is a sample of grabbing the role colours from a role with a gradient.
```cpp
#include <dpp/dpp.h>
#include <dppp/dppp.h>

void role_style(dpp::cluster& bot, dpp::snowflake guild_id, dpp::snowflake role_id) {
    dppp::get_enhanced_role(bot, guild_id, role_id, [](const dppp::result<dppp::enhanced_role>& res) {
        if (res.success) {
            if (res.value.colours.is_gradient()) {
                std::cout << "Woah! Cool gradient: " << res.value.colours.primary_hex_colour() 
                          << " -> " << res.value.colours.secondary_hex_colour().value_or("#000000") 
                          << std::endl;
            }
        }
    });
}
```

-----

## Installation

To put it simply, I would not recommend using this unless you absolutely need to. This was made solely for one of my own projects to provide a stopgap with enhanced role colours. However, if you wish to install it, you will have to build it from source:

1. Download source code:
`git clone https://github.com/joshuazcchen/dpp_enhanced_roles`

2. Create a build directory:
`mkdir build && cd build`

3. Configure and install:
`cmake .. && sudo cmake --install .`

4. Include the package in your respective CMakeLists.txt and use the `#include <dppp/dppp.h>` header.

-----

## Dependencies 

- [D++](https://github.com/brainboxdotcc/DPP/): the core library is required, as this is just an addon.
- A C++17 compatible compiler (GCC 8+).

All other criteria are satisfied by simply having D++ installed.
