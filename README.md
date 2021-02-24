# CLionDeleaker

This is a small utility designed to act as a stub between CLion and Deleaker, it is used by deleaker to open the source file where a memory leak occured in CLion at the correct line.

It brings the CLion project window to the foreground, opens the source file and positions the cursor at the line containing the leak.

## Usage

```shell
cliondeleaker.exe "C:\Program Files\JetBrains\CLion 2020.3\bin\clion64.exe" Pingnoo 100 "C:\Source\File.cpp"
```

This would open the file C:\Source\File.cpp in CLion in the project that contains it, it will position the cursor at line 100.

The app then searches the windows in the system for a CLion window that contains the text "Pingnoo", which in my case is the name of the project.  If it finds a matching window then it will bring that window to the foreground.

## Configuration

You will need Deleaker 2021.5.0.0 or later as the required parameters for launching an external editor are only available in versions equal or later to this.

You should go to options and then select a customer editor with the following format (changing the paths to the this tool and CLion so they match your installation):

```shell
C:\tools\cliondeleaker.exe "C:\Program Files\JetBrains\CLion 2020.3\bin\clion64.exe" Pingnoo $line$ $path$
```

You should now be able to double click on a leak and be taken directly to CLion.

## About Deleaker

Deleaker is a fantastic tool for capturing and profiling memory & handle leaks under Windows, it can be downloaded from https://www.deleaker.com

## About CLion

CLion is an IDE for C++ applications, it can be downloaded from https://www.jetbrains.com/clion

## License

This project is open source and is released under the [MIT License](https://github.com/nedrysoft/cliondeleaker/blob/master/LICENSE.md)

Distributed as-is; no warranty is given.

