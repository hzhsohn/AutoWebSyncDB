@echo off
set a=%~dp0

cd %a%/后台服务
start SyncService_x64.exe -run

cd %a%/客户端
start assembly.exe