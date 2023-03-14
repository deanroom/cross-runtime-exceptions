#include <iostream>
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include "native-exceptions.h"

#include <stddef.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <chrono>

#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>
#include <client/crashpad_info.h>

std::unique_ptr<crashpad::CrashReportDatabase> database;

static bool
startCrashHandler(std::string const& url, std::string const& handler_path,
    std::string const& db_path)
{
    using namespace crashpad;

    std::map<std::string, std::string> annotations;
    std::vector<std::string> arguments;
    bool rc;

    /*
     * THE FOLLOWING ANNOTATIONS MUST BE SET.
     *
     * Backtrace supports many file formats. Set format to minidump
     * so it knows how to process the incoming dump.
     */
    annotations["format"] = "minidump";

    /*
     * REMOVE THIS FOR ACTUAL BUILD.
     *
     * We disable crashpad rate limiting for this example.
     */
    arguments.push_back("--no-rate-limit");

    base::FilePath db(db_path);
    base::FilePath handler(handler_path);

    database = crashpad::CrashReportDatabase::Initialize(db);

    if (database == nullptr || database->GetSettings() == NULL)
        return false;

    /* Enable automated uploads. */
    database->GetSettings()->SetUploadsEnabled(false);

    return CrashpadClient{}.StartHandler(
        handler, db, db, url, annotations, arguments, false, false, {}
    );
}

int main() {

    CLI::App app{"Native Exception Demo"};

    std::string url("");

    std::string handler_path("crashpad/handler/handler");

   
    std::string db_path("./dmpdb");

    std::cout << "Crashpad URL: " << url << '\n';
    std::cout << "Crashpad handler: " << handler_path << '\n';
    std::cout << "Crashpad database: " << db_path << '\n';
    std::cerr << startCrashHandler(url, handler_path, db_path) << '\n';


    //ErrorNumber error_number{ErrorNumber::NormalThrow};
    int error_number = 0;
    app.add_option("-e,--error", error_number, "A specified error number which is\n"
                                               "\t0: normal exception\n"
                                               "\t1: EXC_BAD_ACCESS\n"
                                               "\t2: EXC_CRASH\n"
                                               "\t3: EXCEPTION_STACK_OVERFLOW");

    CLI11_PARSE(app);
    throw_exception((ErrorNumber) error_number);
    return 0;
}
