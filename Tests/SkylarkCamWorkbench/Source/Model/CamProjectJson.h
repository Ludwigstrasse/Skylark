#pragma once

#include "Model/CamProject.h"

#include <filesystem>
#include <string>
#include <string_view>

namespace skylark::cam
{
struct CamProjectJsonResult
{
    bool success{false};
    std::string message;
    CamProject project;
};

std::string SerializeCamProjectToJson(const CamProject& project);
CamProjectJsonResult DeserializeCamProjectFromJson(std::string_view jsonText);
bool SaveCamProjectJson(const std::filesystem::path& path, const CamProject& project, std::string& message);
CamProjectJsonResult LoadCamProjectJson(const std::filesystem::path& path);
}
