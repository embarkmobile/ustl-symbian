@rem Script to verify output after running the Symbian tests.
@rem
@rem This file is part of the ustl library, an STL implementation.
@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
@rem This file is free software, distributed under the MIT License.


rem Set DIFFTOOL to your favourite differences tool.
rem set DIFFTOOL=p4diff -w

set DIFFTOOL=diff

rem set EPOCROOT=\Symbian\UIQ3SDK\

pushd ..\bvt
for %%i in (*.std) do %difftool% %%i %EPOCROOT%\epoc32\winscw\c\%%i
popd
