cmake_minimum_required(VERSION 3.22)
project(cpp_exceptions)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

set(CRASHPAD_DIR ~/src/chromium/crashpad/crashpad)

include_directories(
        ${CRASHPAD_DIR}
        ${CRASHPAD_DIR}/out/Default/gen
        ${CRASHPAD_DIR}/third_party/mini_chromium/mini_chromium
        ${CRASHPAD_DIR}/third_party/mini_chromium/mini_chromium/base
        ${CRASHPAD_DIR}/third_party/mini_chromium/mini_chromium/base/mac
)

link_directories(
        ${CRASHPAD_DIR}/out/Default/obj/client
        ${CRASHPAD_DIR}/out/Default/obj/util
        ${CRASHPAD_DIR}/out/Default/obj/util/file
        ${CRASHPAD_DIR}/out/Default/obj/util/mac
        ${CRASHPAD_DIR}/out/Default/obj/util/mach
        ${CRASHPAD_DIR}/out/Default/obj/util/misc
        ${CRASHPAD_DIR}/out/Default/obj/util/net
        ${CRASHPAD_DIR}/out/Default/obj/util/numeric
        ${CRASHPAD_DIR}/out/Default/obj/util/posix
        ${CRASHPAD_DIR}/out/Default/obj/util/process
        ${CRASHPAD_DIR}/out/Default/obj/util/stdlib
        ${CRASHPAD_DIR}/out/Default/obj/util/stream
        ${CRASHPAD_DIR}/out/Default/obj/util/string
        ${CRASHPAD_DIR}/out/Default/obj/util/synchronization
        ${CRASHPAD_DIR}/out/Default/obj/util/thread
        ${CRASHPAD_DIR}/out/Default/obj/third_party/mini_chromium/mini_chromium
        ${CRASHPAD_DIR}/out/Default/obj/third_party/mini_chromium/mini_chromium/base
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/handler
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/handler/mac
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/tools
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/snapshot
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/snapshot/mac
        /Users/jerryding/src/chromium/crashpad/crashpad/out/Default/obj/minidump

)

#add_library(${PROJECT_NAME}_lib SHARED library.cpp)
add_executable(${PROJECT_NAME} main.cpp library.cpp)
target_link_libraries(
        ${PROJECT_NAME}
        client
        base
        common
        util
        net
        mig_output
        bsm
        z
        util.delimited_file_reader.o
        util.directory_reader_posix.o
        util.file_helper.o
        util.file_io_posix.o
        util.file_io.o
        util.file_reader.o
        util.file_seeker.o
        util.file_writer.o
        util.filesystem_posix.o
        util.output_stream_file_writer.o
        util.scoped_remove_file.o
        util.string_file.o
        util.launchd.o
        util.mac_util.o
        util.service_management.o
        util.sysctl.o
        util.xattr.o
        util.bootstrap.o
        util.child_port_handshake.o
        util.child_port_server.o
        util.composite_mach_message_server.o
        util.exc_client_variants.o
        util.exc_server_variants.o
        util.exception_behaviors.o
        util.exception_ports.o
        util.exception_types.o
        util.mach_extensions.o
        util.mach_message_server.o
        util.mach_message.o
        util.notify_server.o
        util.scoped_task_suspend.o
        util.symbolic_constants_mach.o
        util.task_for_pid.o
        util.capture_context_mac.o
        util.clock_mac.o
        util.clock_posix.o
        util.initialization_state_dcheck.o
        util.lexing.o
        util.metrics.o
        util.paths_mac.o
        util.pdb_structures.o
        util.random_string.o
        util.range_set.o
        util.reinterpret_bytes.o
        util.scoped_forbid_return.o
        util.time.o
        util.uuid.o
        util.zlib.o
        net.http_body_gzip.o
        net.http_body.o
        net.http_multipart_builder.o
        net.http_transport_mac.o
        net.http_transport.o
        net.url.o
        util.checked_address_range.o
        util.close_multiple.o
        util.close_stdio.o
        util.drop_privileges.o
        util.process_info_mac.o
        util.scoped_dir.o
        util.scoped_mmap.o
        util.signals.o
        util.spawn_subprocess.o
        util.symbolic_constants_posix.o
        util.process_memory_mac.o
        util.process_memory_range.o
        util.process_memory.o
        util.aligned_allocator.o
        util.string_number_conversion.o
        util.strlcpy.o
        util.strnlen.o
        util.base94_output_stream.o
        util.file_encoder.o
        util.file_output_stream.o
        util.log_output_stream.o
        util.zlib_output_stream.o
        util.split_string.o
        util.semaphore_mac.o
        util.semaphore_posix.o
        util.thread_log_messages.o
        util.thread_posix.o
        util.thread.o
        util.worker_thread.o
        "-framework Foundation"
        "-framework CoreGraphics"
        "-framework CoreText"
        "-framework Security"
        "-framework IOKit"
)

add_executable(handler handler.cpp)
target_link_libraries(
        handler
        client
        base
        common
        util
        net
        mig_output
        bsm
        z
        common.crash_report_upload_thread.o
        common.minidump_to_upload_parameters.o
        handler.prune_crash_reports_thread.o
        handler.user_stream_data_source.o
        libcommon.a
        libhandler.a
        common.file_limit_annotation.o
        handler.crash_report_exception_handler.o
        handler.exception_handler_server.o
        tool_support.tool_support.o
        snapshot.a
        snapshot.cpu_context_mac.o
        snapshot.exception_snapshot_mac.o
        snapshot.mach_o_image_annotations_reader.o
        snapshot.mach_o_image_reader.o
        snapshot.mach_o_image_segment_reader.o
        snapshot.mach_o_image_symbol_table_reader.o
        snapshot.module_snapshot_mac.o
        snapshot.process_reader_mac.o
        snapshot.process_snapshot_mac.o
        snapshot.process_types.o
        snapshot.system_snapshot_mac.o
        snapshot.thread_snapshot_mac.o
        context.cpu_context.o
        libcontext.a
        libsnapshot.a
        snapshot.annotation_snapshot.o
        snapshot.capture_memory.o
        snapshot.crashpad_info_client_options.o
        snapshot.handle_snapshot.o
        snapshot.memory_snapshot.o
        snapshot.unloaded_module_snapshot.o
        format.minidump_extensions.o
        libformat.a
        libminidump.a
        minidump.minidump_annotation_writer.o
        minidump.minidump_byte_array_writer.o
        minidump.minidump_context_writer.o
        minidump.minidump_crashpad_info_writer.o
        minidump.minidump_exception_writer.o
        minidump.minidump_file_writer.o
        minidump.minidump_handle_writer.o
        minidump.minidump_memory_info_writer.o
        minidump.minidump_memory_writer.o
        minidump.minidump_misc_info_writer.o
        minidump.minidump_module_crashpad_info_writer.o
        minidump.minidump_module_writer.o
        minidump.minidump_rva_list_writer.o
        minidump.minidump_simple_string_dictionary_writer.o
        minidump.minidump_stream_writer.o
        minidump.minidump_string_writer.o
        minidump.minidump_system_info_writer.o
        minidump.minidump_thread_id_map.o
        minidump.minidump_thread_name_list_writer.o
        minidump.minidump_thread_writer.o
        minidump.minidump_unloaded_module_writer.o
        minidump.minidump_user_extension_stream_data_source.o
        minidump.minidump_user_stream_writer.o
        minidump.minidump_writable.o
        minidump.minidump_writer_util.o
        "-framework Foundation"
        "-framework CoreGraphics"
        "-framework CoreText"
        "-framework Security"
        "-framework IOKit"
)

