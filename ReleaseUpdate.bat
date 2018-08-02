//명령어, 옵션, 복사할 경로, 복사받을 경로

xcopy /y	.\Engine\System\Release\System.lib			.\Reference\Library\Release
xcopy /y	.\Engine\System\Release\System.dll			.\Client\Release\

xcopy /y	.\Engine\Utility\Release\Utility.lib		.\Reference\Library\Release
xcopy /y	.\Engine\Utility\Release\Utility.dll		.\Client\Release\