using System.Runtime.InteropServices;

namespace csharp_app;

internal static class NativeMethods
{
    private const string DllName = "liblocal_test_cpp_exceptions_lib";
    [DllImport(DllName, EntryPoint = "normal_throw")]
    internal static extern void NormalThrow();
    
    [DllImport(DllName, EntryPoint = "access_none_initialize")]
    internal static extern void AccessNoneInitialize();
    
    [DllImport(DllName, EntryPoint = "array_out_of_range")]
    internal static extern void ArrayOutOfRange();
    
    [DllImport(DllName, EntryPoint = "pointer_out_of_range")]
    internal static extern void PointerOutOfRange();
    
    [DllImport(DllName, EntryPoint = "char_out_of_range")]
    internal static extern void CharOutOfRange();
    
    [DllImport(DllName, EntryPoint = "iterator_out_of_range")]
    internal static extern void IteratorOutOfRange();
    
    [DllImport(DllName, EntryPoint = "dangling_pointer")]
    internal static extern void DanglingPointer();
    
    [DllImport(DllName, EntryPoint = "wild_pointer")]
    internal static extern void WildPointer();
    
    [DllImport(DllName, EntryPoint = "null_pointer")]
    internal static extern void NullPointer();
    
    [DllImport(DllName, EntryPoint = "heap_corruption")]
    internal static extern void HeapCorruption();
    
        
    [DllImport(DllName, EntryPoint = "delete_pointer_twice")]
    internal static extern void DeletePointerTwice();

        
    [DllImport(DllName, EntryPoint = "infinite_recursion")]
    internal static extern void InfiniteRecursion();

    
}