// See https://aka.ms/new-console-template for more information

using csharp_app;

try
{
    // *EXC_CRASH (SIGABRT) | CATCH SEHException
    // NativeMethods.NormalThrow();

    // *NO CRASH WRONG VALUE
    // NativeMethods.DanglingPointer();

    // *EXC_CRASH (SIGABRT)  malloc: Heap corruption detected, free list is damaged at 0x60000258ce30
    //NativeMethods.HeapCorruption();

    // *EXC_CRASH (SIGABRT) Stack overflow.| 0xc0000005 System.StackOverflowException
    //NativeMethods.InfiniteRecursion();

    // *EXC_CRASH (SIGABRT) | 0x80000003
    //NativeMethods.WildPointer();

    // *EXC_BAD_ACCESS (SIGSEGV) | 0xc0000005 System.AccessViolationException
    // NativeMethods.NullPointer();

    // *NO CRASH WRONG VALUE | 0x80000003
    //NativeMethods.AccessNoneInitialize();

    // *EXC_CRASH (SIGABRT) pointer being freed was not allocated | 0xc0000374
    //NativeMethods.DeletePointerTwice();

    // *NO CRASH WRONG VALUE 
    //NativeMethods.ArrayOutOfRange();

    // *NO CRASH WRONG VALUE 
    //NativeMethods.CharOutOfRange();

    // *NO CRASH WRONG VALUE
    //NativeMethods.IteratorOutOfRange();

    // *NO CRASH WRONG VALUE
    //NativeMethods.PointerOutOfRange();
}
catch (Exception ex)
{
    Console.WriteLine($"Catch Exception:{ex.Message}");
}
Console.WriteLine("Hello, World!");