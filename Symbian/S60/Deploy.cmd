@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
@rem This file is free software, distributed under the MIT License.

rem Build USTL library for emulator (WINSCW UDEB) and target hardware (GCCE UREL)

call bldmake bldfiles
call abld build winscw udeb
call abld build gcce urel


