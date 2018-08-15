::
::  download_3rdparty.bat
::  WSpring
::
::  Created by kimbom on 2017. 12. 06...
::  Copyright 2017 kimbom. All rights reserved.
::

::start
powershell "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;(New-Object System.Net.WebClient).DownloadFile('https://github.com/springkim/TSpring/releases/download/3rdparty/3rdparty.zip','3rdparty.zip')"
powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('3rdparty.zip', '3rdparty'); }"
DEL 3rdparty.zip
pause