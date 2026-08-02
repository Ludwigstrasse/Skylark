#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
#endif

namespace Skylark::FormatWorkbench
{
    class FwMappedFile final
    {
    public:
        FwMappedFile() = default;
        ~FwMappedFile();

        FwMappedFile(const FwMappedFile&) = delete;
        FwMappedFile& operator=(const FwMappedFile&) = delete;

        FwMappedFile(FwMappedFile&& Other) noexcept;
        FwMappedFile& operator=(FwMappedFile&& Other) noexcept;

        bool OpenReadOnly(const std::filesystem::path& Path, std::string& ErrorMessage);
        void Close();

        [[nodiscard]] const std::uint8_t* Data() const { return Bytes; }
        [[nodiscard]] std::uint64_t Size() const { return ByteCount; }
        [[nodiscard]] bool IsOpen() const { return Bytes != nullptr; }

    private:
        const std::uint8_t* Bytes = nullptr;
        std::uint64_t ByteCount = 0;
#if defined(_WIN32)
        HANDLE FileHandle = INVALID_HANDLE_VALUE;
        HANDLE MappingHandle = nullptr;
#else
        int FileDescriptor = -1;
#endif
    };
}
