#include "Report/JsonReportWriter.h"

#include <fstream>
#include <iomanip>

namespace Skylark::FormatWorkbench
{
    namespace
    {
        static void WriteEscaped(std::ostream& Output, const std::string& Text)
        {
            Output << '"';
            for (char C : Text)
            {
                switch (C)
                {
                case '\\': Output << "\\\\"; break;
                case '"': Output << "\\\""; break;
                case '\n': Output << "\\n"; break;
                case '\r': Output << "\\r"; break;
                case '\t': Output << "\\t"; break;
                default: Output << C; break;
                }
            }
            Output << '"';
        }

        static const char* SeverityToString(EImportSeverity Severity)
        {
            switch (Severity)
            {
            case EImportSeverity::Warning: return "warning";
            case EImportSeverity::Error: return "error";
            default: return "info";
            }
        }
    }

    bool WriteImportReportJson(std::ostream& Output, const FwImportReport& Report, const FwModelDocument* Document)
    {
        Output << "{\n";
        Output << "  \"succeeded\": " << (Report.Succeeded ? "true" : "false") << ",\n";
        Output << "  \"format\": "; WriteEscaped(Output, ToString(Report.Format)); Output << ",\n";
        Output << "  \"fileBytes\": " << Report.FileBytes << ",\n";
        Output << "  \"triangles\": " << Report.Triangles << ",\n";
        Output << "  \"vertices\": " << Report.Vertices << ",\n";
        Output << "  \"meshBlocks\": " << Report.MeshBlocks << ",\n";
        Output << "  \"compactCpuBytes\": " << Report.CompactCpuBytes << ",\n";
        Output << "  \"importMilliseconds\": " << std::fixed << std::setprecision(3) << Report.ImportMilliseconds << ",\n";
        Output << "  \"memory\": {\n";
        Output << "    \"beforeResidentBytes\": " << Report.MemoryBefore.ResidentBytes << ",\n";
        Output << "    \"afterResidentBytes\": " << Report.MemoryAfter.ResidentBytes << ",\n";
        Output << "    \"afterPeakResidentBytes\": " << Report.MemoryAfter.PeakResidentBytes << "\n";
        Output << "  },\n";
        if (Document != nullptr)
        {
            Output << "  \"bounds\": {\n";
            Output << "    \"min\": [" << Document->Bounds.Min.X << ", " << Document->Bounds.Min.Y << ", " << Document->Bounds.Min.Z << "],\n";
            Output << "    \"max\": [" << Document->Bounds.Max.X << ", " << Document->Bounds.Max.Y << ", " << Document->Bounds.Max.Z << "]\n";
            Output << "  },\n";
        }
        Output << "  \"messages\": [\n";
        for (std::size_t I = 0; I < Report.Messages.size(); ++I)
        {
            const FwImportMessage& Message = Report.Messages[I];
            Output << "    { \"severity\": "; WriteEscaped(Output, SeverityToString(Message.Severity)); Output << ", \"text\": "; WriteEscaped(Output, Message.Text); Output << " }";
            if (I + 1 < Report.Messages.size())
                Output << ',';
            Output << "\n";
        }
        Output << "  ]\n";
        Output << "}\n";
        return static_cast<bool>(Output);
    }

    bool WriteImportReportJsonFile(const std::filesystem::path& Path, const FwImportReport& Report, const FwModelDocument* Document, std::string& ErrorMessage)
    {
        std::ofstream Output(Path);
        if (!Output)
        {
            ErrorMessage = "Cannot create report file.";
            return false;
        }
        if (!WriteImportReportJson(Output, Report, Document))
        {
            ErrorMessage = "Failed while writing report file.";
            return false;
        }
        return true;
    }
}
