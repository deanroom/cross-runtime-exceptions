//
// Created by Jerry Ding on 2023/3/11.
//

#ifndef NATIVE_EXCEPTIONS_NATIVE_EXCEPTIONS_H
#define NATIVE_EXCEPTIONS_NATIVE_EXCEPTIONS_H

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
// used by C++ source code
#endif
enum class ErrorNumber : int {
    NormalThrow, NullPointer, DeletePointerTwice, InfiniteRecursion
};

void throw_exception(ErrorNumber number);
#ifdef __cplusplus
}
#endif

#endif //NATIVE_EXCEPTIONS_NATIVE_EXCEPTIONS_H
