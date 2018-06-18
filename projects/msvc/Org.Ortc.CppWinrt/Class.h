#pragma once

#include "Class.g.h"

namespace winrt::Org_Ortc_CppWinrt::implementation
{
    struct Class : ClassT<Class>
    {
        Class() = default;

        int32_t Dummy();
        void Dummy(int32_t value);
    };
}

namespace winrt::Org_Ortc_CppWinrt::factory_implementation
{
    struct Class : ClassT<Class, implementation::Class>
    {
    };
}
