#include "Core/MappedFile.h"

#include <cerrno>
#include <cstring>
#include <system_error>

#if !defined(_WIN32)
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace Skylark::FormatWorkbench
{
    FwMappedFile::~FwMappedFile()
    {
        Close();
    }

    FwMappedFile::FwMappedFile(FwMappedFile&& Other) noexcept
    {
        *this = std::move(Other);
    }

    FwMappedFile& FwMappedFile::operator=(FwMappedFile&& Other) noexcept
    {
        if (this == &Other)
            return *this;

        Close();
        Bytes = Other.Bytes;
        ByteCount = Other.ByteCount;
        Other.Bytes = nullptr;
        Other.ByteCount = 0;
#if defined(_WIN32)
        FileHandle = Other.FileHandle;
        MappingHandle = Other.MappingHandle;
        Other.FileHandle = INVALID_HANDLE_VALUE;
        Other.MappingHandle = nullptr;
#else
        FileDescriptor = Other.FileDescriptor;
        Other.FileDescriptor = -1;
#endif
        return *this;
    }

    bool FwMappedFile::OpenReadOnly(const std::filesystem::path& Path, std::string& ErrorMessage)
    {
        Close();
        std::error_code Ec;
        const auto FileSize = std::filesystem::file_size(Path, Ec);
        if (Ec)
        {
            ErrorMessage = "Cannot query file size: " + Ec.message();
            return false;
        }
        if (FileSize == 0)
        {
            ErrorMessage = "File is empty.";
            return false;
        }
        ByteCount = static_cast<std::uint64_t>(FileSize);

#if defined(_WIN32)
        FileHandle = CreateFileW(Path.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (FileHandle == INVALID_HANDLE_VALUE)
        {
            ErrorMessage = "CreateFileW failed.";
            Close();
            return false;
        }

        MappingHandle = CreateFileMappingW(FileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (MappingHandle == nullptr)
        {
            ErrorMessage = "CreateFileMappingW failed.";
            Close();
            return false;
        }

        Bytes = static_cast<const std::uint8_t*>(MapViewOfFile(MappingHandle, FILE_MAP_READ, 0, 0, 0));
        if (Bytes == nullptr)
        {
            ErrorMessage = "MapViewOfFile failed.";
            Close();
            return false;
        }
#else
        FileDescriptor = open(Path.c_str(), O_RDONLY);
        if (FileDescriptor < 0)
        {
            ErrorMessage = "open failed: ";
            ErrorMessage += std::strerror(errno);
            Close();
            return false;
        }

        void* Mapped = mmap(nullptr, static_cast<std::size_t>(ByteCount), PROT_READ, MAP_PRIVATE, FileDescriptor, 0);
        if (Mapped == MAP_FAILED)
        {
            ErrorMessage = "mmap failed: ";
            ErrorMessage += std::strerror(errno);
            Close();
            return false;
        }
        Bytes = static_cast<const std::uint8_t*>(Mapped);
#endif
        return true;
    }

    void FwMappedFile::Close()
    {
#if defined(_WIN32)
        if (Bytes != nullptr)
        {
            UnmapViewOfFile(Bytes);
            Bytes = nullptr;
        }
        if (MappingHandle != nullptr)
        {
            CloseHandle(MappingHandle);
            MappingHandle = nullptr;
        }
        if (FileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(FileHandle);
            FileHandle = INVALID_HANDLE_VALUE;
        }
#else
        if (Bytes != nullptr)
        {
            munmap(const_cast<std::uint8_t*>(Bytes), static_cast<std::size_t>(ByteCount));
            Bytes = nullptr;
        }
        if (FileDescriptor >= 0)
        {
            close(FileDescriptor);
            FileDescriptor = -1;
        }
#endif
        ByteCount = 0;
    }
}
