@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
cd /d "D:\tmp\xastrix-l4d2-internal-base"
msbuild l4d2-internal-base.sln /p:Configuration=Release /p:Platform=x86 /m /v:n
