## This file is a template, the comment below is emitted into the generated file
/* This is an auto generated file. Do not edit. */
#pragma once

#include <array>
#include <string>

using namespace std::string_literals;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

struct ConfigMeta
{
    using Meta = std::array<std::string, ${len(meta)}>;

    static auto& get()
    {
        static const Meta meta =
        {
% for m in meta:
            "${m.name}"s,
% endfor
        };
        return meta;
    }
};

struct ConfigPaths
{
    using Paths = std::array<std::string, ${len(paths)}>;

    static auto& get()
    {
        static const Paths paths =
        {
% for p in paths:
            "${p.name}"s,
% endfor
        };
        return paths;
    }
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
