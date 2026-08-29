#pragma once

#include <stdexcept>
#include <wrl/wrappers/corewrappers.h>
#include <string>

namespace gr::rhi
{

// various dx12 utility functions
inline DXGI_FORMAT ToDXGIFormat(GrFormat fmt)
{
    switch (fmt)
    {
    case GrFormat::R8G8B8A8_UNORM:          return DXGI_FORMAT_R8G8B8A8_UNORM;
    case GrFormat::R8_UNORM:                return DXGI_FORMAT_R8_UNORM;
    case GrFormat::R8_SNORM:                return DXGI_FORMAT_R8_SNORM;

    case GrFormat::R32G32B32_UINT:          return DXGI_FORMAT_R32G32B32_UINT;
    case GrFormat::R32G32B32_SINT:          return DXGI_FORMAT_R32G32B32_SINT;
    case GrFormat::R32G32B32_SFLOAT:        return DXGI_FORMAT_R32G32B32_FLOAT;

    case GrFormat::R32G32B32A32_UINT:       return DXGI_FORMAT_R32G32B32A32_UINT;
    case GrFormat::R32G32B32A32_SINT:       return DXGI_FORMAT_R32G32B32A32_SINT;
    case GrFormat::R32G32B32A32_SFLOAT:     return DXGI_FORMAT_R32G32B32A32_FLOAT;

    case GrFormat::D32_SFLOAT:              return DXGI_FORMAT_D32_FLOAT;

    case GrFormat::UNDEFINED:
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

inline bool IsDepthStencilFormat(DXGI_FORMAT fmt)
{
    switch (fmt)
    {
    // Pure depth formats
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_D32_FLOAT:    
    // Depth + stencil formats
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    // Typeless versions (used for creating SRV/DSV pairs)
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return true;

    // Non-depthstencil format
    default:
        return false;
    }
}

inline bool IsDepthStencilFormat(GrFormat fmt)
{
    return IsDepthStencilFormat(ToDXGIFormat(fmt));
}

inline std::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}

class HrException : public std::runtime_error
{
    public:
        HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
        HRESULT Error() const { return m_hr; }
    private:
        const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}

inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
{
    using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
    CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
    extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
    extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
    extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
    extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
    extendedParams.lpSecurityAttributes = nullptr;
    extendedParams.hTemplateFile = nullptr;

    Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
#else
    Wrappers::FileHandle file(CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, nullptr));
#endif
    if (file.Get() == INVALID_HANDLE_VALUE)
    {
        throw std::exception();
    }

    FILE_STANDARD_INFO fileInfo = {};
    if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
    {
        throw std::exception();
    }

    if (fileInfo.EndOfFile.HighPart != 0)
    {
        throw std::exception();
    }

    *data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
    *size = fileInfo.EndOfFile.LowPart;

    if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
    {
        throw std::exception();
    }

    return S_OK;
}

#ifdef D3D_COMPILE_STANDARD_FILE_INCLUDE
inline Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
    const std::wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const std::string& entrypoint,
    const std::string& target)
{
    UINT compileFlags = 0;
#if defined(_DEBUG) || defined(DBG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr;

    Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errors;
    hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

    if (errors != nullptr)
    {
        OutputDebugStringA((char*)errors->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    return byteCode;
}
#endif

}
