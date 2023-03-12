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
startCrashHandler(std::string const &url, std::string const &handler_path,
                  std::string const &db_path) {
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

    std::cerr << __LINE__ << '\n';

    if (database == nullptr || database->GetSettings() == NULL)
        return false;

    std::cerr << __LINE__ << '\n';

    /* Enable automated uploads. */
    database->GetSettings()->SetUploadsEnabled(false);
    return CrashpadClient{}.StartHandler(
            handler, db, db, url, annotations, arguments, false, false, {}
    );
}

void Crash() {
    memset((void *) (intptr_t) 4, 123, 1);
}

void InitCrashpad() {
    /*
     * YOU MUST CHANGE THIS VALUE.
     *
     * This should point to your server dump submission port
     * (labeled as "http/writer" in the listener configuration
     * pane. Preferably, the SSL enabled port should
     * be used.
     */
    std::string url(
            "https://submit.backtrace.io/yolo/c5a3a1675dbcfa1d12fd7efbc7bef9350874654e55ff86478115c8bc073f54dc/minidump");

    /*
     * ENSURE THIS VALUE IS CORRECT.
     *
     * Crashpad has the ability to support crashes both in-process
     * and out-of-process. The out-of-process handler is
     * significantly more robust than traditional in-process crash
     * handlers. This path may be relative.
     */
    std::string handler_path("./handler");

    /*
     * ENSURE THIS VALUE IS CORRECT.
     *
     * This is the directory you will use to store and
     * queue crash data.
     */
    std::string db_path("/Users/jerryding/crashpad");

//    if (argc > 1) url = argv[1];
//    if (argc > 2) handler_path = argv[2];
//    if (argc > 3) db_path = argv[3];

    std::cout << "Crashpad URL: " << url << '\n';
    std::cout << "Crashpad handler: " << handler_path << '\n';
    std::cout << "Crashpad database: " << db_path << '\n';
    std::cerr << startCrashHandler(url, handler_path, db_path) << '\n';
}

int main() {
    InitCrashpad();
//    normal_throw();
//    access_none_initialize();
//    array_out_of_range();
//    pointer_out_of_range();
//    char_out_of_range();
//    iterator_out_of_range();
//    dangling_pointer();
    wild_pointer();
//    null_pointer();
//    heap_corruption();
//    delete_pointer_twice();
//    infinite_recursion();
    return 0;
}