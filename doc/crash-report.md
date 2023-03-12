# Crash Report-跨运行时程序崩溃问题及方案讨论

## 问题

### Phoenix 5.X(.NET Framework 4.5) 现状

Phoenix 软件 5.x 中一直存在**非托管代码导致的软件崩溃**跟踪处理的困难。集中表现在**用户现场**或**测试环境下**出现崩溃后，技术人员拿到的信息对于问题排查帮助有限。当前Phoenix 5.x软件能获取到的信息包括应用程序日志、Windows事件中心日志、dump 文件等。其中

- 5.x 软件提供的日志记录最多只反应到**寻址调用**，内部细节（模块、代码行等信息）无法体现
- Windows 事件中心能提供对应的错误编码（005,374），模块名称为coreclr.dll.
- 当前Phoenix 在非托管代码造成崩溃后Framework 中通过 GlobalUnhandledException 的方式**并不总能生成dump文件**，主要体现在非托管异常并没有被捕获。（需确认仅是程序 bug 还是其他问题）
- NcCore 中针对每个 P/Invoke 增加[HandleProcessCorruptedStateExceptionsAttribute](https://learn.microsoft.com/zh-cn/dotnet/api/system.runtime.exceptionservices.handleprocesscorruptedstateexceptionsattribute?view=net-7.0)的方式进行日志记录并生成 dump，此机制通过环境变量进行开关，仅在测试环境和现场排查时用来记录崩溃时的 C#调用堆栈和 dump 文件。

### Phoenix 6.X(.NET 6.0)中对于非托管异常的处理

以上问题在 Phoenix6.x 中依然存在，同时由于.NET升级到 6.0 后对于异常从非托管代码到托管代码的传递策略有了一些变化。原有 P/INVOKE 通过增加 [HandleProcessCorruptedStateExceptionsAttribute](https://learn.microsoft.com/zh-cn/dotnet/api/system.runtime.exceptionservices.handleprocesscorruptedstateexceptionsattribute?view=net-7.0)捕获非托管异常的方式已废弃，通过代码验证可知：所有**非 C++标准异常（系统异常）均不能通过try/catch**的方式捕获,标准 C++异常可以**在 Windows 平台下通过 SEHException捕获**。

## 分析

目前的核心问题是Phoenix 随着软件迭代在应用程序级别的**全局未处理异常机制**已不能用来快速处理软件崩溃问题，尤其是**跨运行时的未处理异常**的处理。

### 当前异常分类

-  可捕获异常（try/catch,UnhandledException）
  - 所有**托管异常**（可以捕获异常）
  - **标准C++异常**（Windows 可以捕获并转换为 SEHException,Mac/Linux 下不能捕获任意异常）
- 不可捕获异常
  - **系统异常**


### 造成Phoenix崩溃的主要是未处理的系统异常

- EXCEPTION_ACCESS_VIOLATION 0xc0000**005**
- HEAP CORRUPTION 0xC0000**374**
- EXCEPTION_STACK_OVERFLOW     0xC0000**0FD**

- STATUS_NO_MEMORY 0XC0000**017**

### 涉及模块

- 解析器
- MCC（5.x）
- 轨迹
- NcEditor

### 对于跨运行时异常的一些调研

- [Runtime Exception Propagation](https://www.mono-project.com/docs/advanced/pinvoke/#runtime-exception-propagation)（Mono)

  - P/Invoke assumes that the unmanaged code conforms to the **C ABI**. C doesn’t support exceptions. As such, it is **assumed that runtime exceptions will not propagate through unmanaged code**.

  - Microsoft Visual C++ uses SEH to implement C++ exception handling and supports the use of SEH in C as a language extension through the __try, __except, and __finally keywords. SEH is a Microsoft extension; **it does not exist outside of Microsoft and .NET,** and as such is not portable.

  - 结论

    - As long as managed and unmanaged code use different exception handling mechanisms, **exceptions *must not* be mixed between them.**

      The moral of this story: **don’t let exceptions propagate between managed and unmanaged code**. The results won’t be pretty.

      This is particularly pertinent when wrapping C++ methods. C++ exceptions will need to be mapped into an “out” parameter or a return value, so that managed code can know what error occurred, and (optionally) throw a managed exception to “propagate” the original C++ exception.

- [SEH Exception](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-6.0)

  - The [SEHException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-6.0) class handles SEH errors that are thrown from unmanaged code, but that have not been mapped to another .NET Framework exception. The [SEHException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-6.0) class also corresponds to the HRESULT E_FAIL (0x80004005).

    **The .NET Framework often encounters unmanaged SEH exceptions** that are automatically mapped to managed equivalents. There are two common unmanaged SEH exceptions：...

- .NET对于非托管异常的支持，现状：微软关于交互相异常的文档描述基本表达的还是 **NET Framework的理念和规则**

  - [System.Runtime.InteropServices.ExternalException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.externalexception?view=net-7.0)

    To enhance interoperability between legacy systems and the common language runtime, the [ErrorCode](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.externalexception.errorcode?view=net-7.0) property stores an integer value (HRESULT) that identifies the error. User defined exceptions should never derive from `ExternalException`, and an `ExternalException` should never be thrown by user code. Use the specific exceptions that derive from `ExternalException` instead.

    `ExternalException` uses the HRESULT E_FAIL which has the value 0x80004005.


  - [System.Runtime.InteropServices.SEHException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-7.0)

    The [SEHException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-7.0) class handles SEH errors that are thrown from unmanaged code, but that have not been mapped to another .NET Framework exception. The [SEHException](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.sehexception?view=net-7.0) class also corresponds to the `HRESULT` `E_FAIL` (0x80004005).


  - [System.AccessViolationException](https://learn.microsoft.com/en-us/dotnet/api/system.accessviolationexception?view=net-7.0)


  - [System.OutOfMemoryException](https://learn.microsoft.com/en-us/dotnet/api/system.outofmemoryexception?view=net-7.0) 


  - [System.InsufficientMemoryException](https://learn.microsoft.com/en-us/dotnet/api/system.insufficientmemoryexception?view=net-7.0) 


  - [System.ComponentModel.Win32Exception](https://learn.microsoft.com/en-us/dotnet/api/system.componentmodel.win32exception?view=net-7.0)


## 一些解决方案

针对我们碰到的问题，调研了一些对此问题的解决方案。

### 操作系统级别

- Windows 错误报告 [Windows Error Reporting](https://learn.microsoft.com/en-us/windows/win32/wer/windows-error-reporting)

- Xcode使用的错误报告和设备记录 [Diagnosing issues using crash reports and device logs](https://developer.apple.com/documentation/xcode/diagnosing-issues-using-crash-reports-and-device-logs)

- Ubuntu 错误报告[Ubuntu CrashReporting](https://wiki.ubuntu.com/CrashReporting) 

### 应用程序级别

### [crashpad](https://chromium.googlesource.com/crashpad/crashpad) 

Crashpad is a client-side library that focuses on capturing machine and program state in a postmortem crash report, and transmitting this report to a backend server - a “collection server”. The Crashpad library is embedded by the client application. Conceptually, Crashpad breaks down into the handler and the client. The handler runs **in a separate process from the client or clients.** It is responsible for snapshotting the crashing client process’ state on a crash, saving it to a crash dump, and transmitting the crash dump to an upstream server. Clients register with the handler to allow it to capture and upload their crashes.

### [breakpad](https://chromium.googlesource.com/breakpad/breakpad)

Breakpad provides client libraries for each of its target platforms. Currently, these exist for Windows on x86 and Mac OS X on both x86 and PowerPC. A Linux implementation has been written and is currently under review.

## 提议：Phoenix Error Reporting

### 必要性

实现错误报告机制是提升可维护性的有效手段，并最终来保证我们软件的可靠性，对于一款工控产品，我们要求极高的可靠性，我们已经通过严格的方案设计和代码质量的管控来保证可靠性，同时鉴于我们软件复杂度较高，我们也应该有完整的错误报告机制来提供保障，对于生成环境和测试环境下碰到的意外软件崩溃问题能提供丰富且必要的现场信息来快速识别并修复问题。

### 处理原则

非托管代码应独自处理 C++异常和相关系统异常，这是所有 C++库应该执行的标准。

dubhe 应接管所有托管运行时的未处理异常。

### 可选方案

#### 非托管代码处理所有异常

- 对于跨越 C ABI 的非托管模块应该处理所有 C++异常，并进行日志记录、
- CAPI 内部应该处理所有系统异常？？

#### 使用Crashpad用来处理系统异常

Phoenix 集成 crashpad client 把未处理异常的处理交由 crashpad 接管，crashpad client 向 crash handler注册并交由 crash hander使用独立进程来监控被守护程序的未处理异常。

##### The Crashpad handler

The Crashpad handler is instantiated in a process supplied by the embedding application. It provides means for clients to register themselves by some means of IPC, or where operating system support is available, by taking advantage of such support to cause crash notifications to be delivered to the handler. On crash, the handler snapshots the crashed client process’ state, writes it to a postmortem dump in a database, and may also transmit the dump to an upstream server if so configured.

The Crashpad handler is able to handle cross-bitted requests and generate crash dumps across bitness, where e.g. the handler is a 64-bit process while the client is a 32-bit process or vice versa. In the case of Windows, this is limited by the OS such that a 32-bit handler can only generate crash dumps for 32-bit clients, but a 64-bit handler can acquire nearly all of the detail for a 32-bit process.

##### The Crashpad client

The Crashpad client provides two main facilities.

1. Registration with the Crashpad handler.
2. Metadata communication to the Crashpad handler on crash.

A Crashpad embedder links the Crashpad client library into one or more executables, whether a loadable library or a program file. The client process then registers with the Crashpad handler through some mode of IPC or other operating system-specific support.

On crash, metadata is communicated to the Crashpad handler via the CrashpadInfo structure. Each client executable module linking the Crashpad client library embeds a CrashpadInfo structure, which can be updated by the client with whatever state the client wishes to record with a crash.



