@echo off
title The Paladin
mode con: cols=120 lines=40
reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1 /f >nul
color 0F
"a.exe"