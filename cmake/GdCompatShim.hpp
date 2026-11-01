#pragma once

// GD 2.2091's bindings dropped the gd::string / gd::vector / etc. wrapper types in
// favor of plain std:: equivalents (see geode-sdk/bindings: bindings/2.2091/Extras.bro
// vs bindings/2.2081/Extras.bro - every `gd::string` field became `std::string`).
// Because no .bro file for 2.2091 references the gd:: types anymore, codegen has no
// reason to emit the `gd` namespace at all, so any code that still spells out
// gd::string/gd::vector/etc. explicitly (written against older GD versions) fails to
// compile. This provides those names as plain aliases to their std:: equivalents,
// which is exactly what 2.2091's own bindings use under the hood anyway.
#if defined(GEODE_COMP_GD_VERSION) && GEODE_COMP_GD_VERSION >= 22091

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace gd {
    using string = std::string;
    template <class T> using vector = std::vector<T>;
    template <class K, class V> using map = std::map<K, V>;
    template <class T> using set = std::set<T>;
    template <class K, class V> using unordered_map = std::unordered_map<K, V>;
    template <class T> using unordered_set = std::unordered_set<T>;
}

#endif
