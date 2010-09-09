@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
rem Build the emulator software and test software
rem Use SymbianVerifyOutput (editing for difference tool) to check results.

call bldmake bldfiles
call abld build winscw udeb
call abld test build winscw udeb

