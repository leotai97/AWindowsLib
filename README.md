# AWindowsLib
A c++ library that encapsulates common window controls.  It's targeted to Windows 10 and could possibly be cloned for Linux.

This library contains a class ProseUnit which enables multiple languages without having to rely on Microsoft's technique of create additional .RC files and DLLs.  Additional languages can be easily added by end users using my GitHub project AWindowsProse.

See the project PicMan for examples using this library.

A lot of work went into the ListView class which is available in both dialogs and popup windows.  
The library has a custom splitter control for large resizable windows which can automatically size it's children.

I tried to keep Win32 API calls in this library and out of PicMan as possible in hopes that someday PicMan might be available on Linux.  PicMan does rely on Windows messages, Linux might not be an option.

