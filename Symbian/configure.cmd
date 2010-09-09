@rem Configure.cmd :
@rem DOS command script to set up USTL for Symbian 9 compilation.
@rem Use 
@rem      Configure -u 
@rem to revert files back to original places for source code control.
@rem Note - Should call again after changing any header file.

@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>

@rem This file is free software, distributed under the MIT License.

@rem Standard EPOC32\include directory
set INCDIR=%EPOCROOT%\EPOC32\INCLUDE\ustl

@rem List of dummy header files for standards compatibility:
set STL_FILES=algorithm bitset iostream iterator list map numeric queue set
set STL_FILES=%STL_FILES% stack string strstream utility vector
set SYMBIAN_FILES=stdint.h float.h config.h typeinfo

@rem Sanity check:
if not exist %EPOCROOT%\EPOC32\INCLUDE echo EPOCROOT not set?  && goto exit

if /I "%1" == "-u" goto unconfigure
echo Configuring...

@rem Move the Symbian-specific header files into the main directory.
for %%i in (%SYMBIAN_FILES%) do copy %%i .. && del %%i

@rem Set up the include directory:

mkdir %INCDIR%\USTL
for %%i in (%STL_FILES%) do echo #include "ustl\ustl.h" > %INCDIR%\%%i
copy ..\*.h %INCDIR%
for %%i in (%SYMBIAN_FILES%) do copy ..\%%i %INCDIR%

goto exit

:unconfigure

echo Unconfiguring

@rem rename/copy all files back to original locations for SCC.
pushd ..
for %%i in (%SYMBIAN_FILES%) do copy %%i Symbian && del %%i
popd

@rem Cleanup include directory:
rmdir /q /s %INCDIR%

:exit
