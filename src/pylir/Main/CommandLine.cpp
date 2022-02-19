#include "CommandLine.hpp"

#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>

#include <utility>

#define PREFIX(NAME, VALUE) static const char* const NAME[] = VALUE;
#include <pylir/Main/Opts.inc>
#undef PREFIX

// Don't have much choice until this is fixed in LLVM
using llvm::opt::HelpHidden;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
static const llvm::opt::OptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM, HELPTEXT, METAVAR, VALUES) \
    {PREFIX,                                                                                             \
     NAME,                                                                                               \
     HELPTEXT,                                                                                           \
     METAVAR,                                                                                            \
     ::pylir::cli::OPT_##ID,                                                                             \
     llvm::opt::Option::KIND##Class,                                                                     \
     PARAM,                                                                                              \
     FLAGS,                                                                                              \
     ::pylir::cli::OPT_##GROUP,                                                                          \
     ::pylir::cli::OPT_##ALIAS,                                                                          \
     ALIASARGS,                                                                                          \
     VALUES},
#include <pylir/Main/Opts.inc>
#undef OPTION
};

pylir::cli::PylirOptTable::PylirOptTable() : llvm::opt::OptTable(InfoTable) {}

pylir::cli::CommandLine::CommandLine(std::string exe, int argc, char** argv)
    : m_saver(m_allocator),
      m_args(m_table.parseArgs(argc, argv, OPT_UNKNOWN, m_saver,
                               [this](llvm::StringRef msg)
                               {
                                   m_errorsOccurred = true;
                                   llvm::errs() << pylir::Diag::formatLine(pylir::Diag::Severity::Error, msg);
                               })),
      m_exe(std::move(exe)),
      m_rendered(
          [this]
          {
              std::string rendered = llvm::sys::path::filename(m_exe).str();
              for (auto* iter : m_args)
              {
                  auto arg = iter->getAsString(m_args);
                  rendered += " ";
                  m_argRanges.insert({iter, {rendered.size(), rendered.size() + arg.size()}});
                  rendered += arg;
              }
              return rendered;
          }(),
          "<command-line>")
{
}

void pylir::cli::CommandLine::printHelp(llvm::raw_ostream& out) const
{
    m_table.printHelp(out, (llvm::sys::path::filename(m_exe) + " [options] <input>").str().c_str(),
                      "Optimizing Python compiler using MLIR and LLVM");
}

void pylir::cli::CommandLine::printVersion(llvm::raw_ostream& out) const
{
    out << "pylir " PYLIR_VERSION "\n";
    out << "LLVM " LLVM_VERSION_STRING "\n";
    out << "MLIR " LLVM_VERSION_STRING "\n";
#ifdef PYLIR_EMBEDDED_LLD
    out << "lld " LLVM_VERSION_STRING "\n";
#endif
}

bool pylir::cli::CommandLine::verbose() const
{
    return m_args.hasArg(OPT_verbose);
}

bool pylir::cli::CommandLine::onlyPrint() const
{
    return m_args.hasArg(OPT__HASH_HASH_HASH);
}
