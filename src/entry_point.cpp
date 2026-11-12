#include "core/application.h"
#include "editor/editor_application.h"
#include "rhi/interface/rhi.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

namespace
{
RHI_BACKEND DefaultBackend()
{
#if defined(_WIN32)
    return RHI_BACKEND::D3D12;
#elif defined(__APPLE__)
    return RHI_BACKEND::METAL;
#else
    return RHI_BACKEND::CPU;
#endif
}

std::string Lowercase(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

bool ParseBackend(const std::string& value, RHI_BACKEND& backend)
{
    const std::string name = Lowercase(value);
    if (name == "cpu")
    {
        backend = RHI_BACKEND::CPU;
        return true;
    }

#if defined(__APPLE__)
    if (name == "metal")
    {
        backend = RHI_BACKEND::METAL;
        return true;
    }
#endif

#if defined(_WIN32)
    if (name == "d3d12")
    {
        backend = RHI_BACKEND::D3D12;
        return true;
    }
#endif
    return false;
}

void PrintUsage(const char* program)
{
#if defined(_WIN32)
    constexpr const char* choices = "cpu|d3d12";
#elif defined(__APPLE__)
    constexpr const char* choices = "cpu|metal";
#else
    constexpr const char* choices = "cpu";
#endif
    std::cout << "Usage: " << program << " [-rhi <" << choices << ">]\n";
}
}

int main(int argc, char** argv)
{
    RHI_BACKEND backend = DefaultBackend();
    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            PrintUsage(argv[0]);
            return 0;
        }

        std::string value;
        if (arg == "-rhi" || arg == "--rhi")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Missing backend after " << arg << "\n";
                PrintUsage(argv[0]);
                return 2;
            }
            value = argv[++i];
        }
        else if (arg.rfind("-rhi=", 0) == 0)
        {
            value = arg.substr(5);
        }
        else if (arg.rfind("--rhi=", 0) == 0)
        {
            value = arg.substr(6);
        }
        else
        {
            std::cerr << "Unknown option: " << arg << "\n";
            PrintUsage(argv[0]);
            return 2;
        }

        if (!ParseBackend(value, backend))
        {
            std::cerr << "RHI '" << value << "' is unavailable on this platform.\n";
            PrintUsage(argv[0]);
            return 2;
        }
    }

    gr::EditorApplication app({ "GrEditor", 1600, 900, backend });
    app.Run();
    return 0;
}
