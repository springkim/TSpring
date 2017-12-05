::
::  download_3rdparty.bat
::  WSpring
::
::  Created by kimbom on 2017. 12. 06...
::  Copyright 2017 kimbom. All rights reserved.
::

::start
powershell "(New-Object System.Net.WebClient).DownloadFile('https://www.dropbox.com/s/sz5ofmhknkug5gx/3rdparty.zip?dl=1','3rdparty.zip')"
powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('3rdparty.zip', '3rdparty'); }"
DEL 3rdparty.zip