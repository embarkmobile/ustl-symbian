@rem Created by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
rem Build the target library and test software

call bldmake bldfiles
call abld build gcce urel
call abld test build gcce urel
makesis UstlTest.pkg
signsis UstlTest.sis UstlTest.sisx ..\UstlTest.cer ..\UstlTest.key

