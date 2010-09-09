@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
@rem This file is free software, distributed under the MIT License.

rem Clean up all the USTL derived files.

call abld clean gcce urel
call abld reallyclean gcce urel
call abld test clean gcce urel
call abld test reallyclean gcce urel

call abld clean
call abld reallyclean
call abld test clean
call abld test reallyclean

call bldmake clean
del ustltest.sis

