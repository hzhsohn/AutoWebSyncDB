rem "Çå³ýSDK²¿·Ö"

del "demo\tcp_serv\*.ncb"
del "demo\tcp_serv\*.suo" /a
del "demo\tcp_serv\*.sdf" /a
del "demo\tcp_serv\*.user"
del "demo\tcp_serv\*.o"
del "demo\tcp_serv\*.*~"
rd "demo\tcp_serv\debug" /s/q
rd "demo\tcp_serv\Release" /s/q

del "demo\tcp_client\*.ncb"
del "demo\tcp_client\*.suo" /a
del "demo\tcp_client\*.sdf" /a
del "demo\tcp_client\*.user"
del "demo\tcp_client\*.o"
del "demo\tcp_client\*.*~"
rd "demo\tcp_client\debug" /s/q
rd "demo\tcp_client\Release" /s/q

del "demo\tcp_client_c++\*.ncb"
del "demo\tcp_client_c++\*.suo" /a
del "demo\tcp_client_c++\*.sdf" /a
del "demo\tcp_client_c++\*.user"
del "demo\tcp_client_c++\*.o"
del "demo\tcp_client_c++\*.*~"
rd "demo\tcp_client_c++\debug" /s/q
rd "demo\tcp_client_c++\Release" /s/q

del "demo\udp_serv\*.ncb"
del "demo\udp_serv\*.suo" /a
del "demo\udp_serv\*.sdf" /a
del "demo\udp_serv\*.user"
del "demo\udp_serv\*.o"
del "demo\udp_serv\*.*~"
rd "demo\udp_serv\debug" /s/q
rd "demo\udp_serv\Release" /s/q

del "demo\udp_client\*.ncb"
del "demo\udp_client\*.suo" /a
del "demo\udp_client\*.sdf" /a
del "demo\udp_client\*.user"
del "demo\udp_client\*.o"
del "demo\udp_client\*.*~"
rd "demo\udp_client\debug" /s/q
rd "demo\udp_client\Release" /s/q

del "include\*.o"
del "include\*.*~"
del "src\*.o"
del "src\*.*~"

del "include\http\*.o"
del "include\http\*.*~"
del "src\http\*.o"
del "src\http\*.*~"

del "include\tcp_c++\*.o"
del "include\tcp_c++\*.*~"
del "src\tcp_c++\*.o"
del "src\tcp_c++\*.*~"

del "..\bin\debug\*.exp"
del "..\bin\debug\*.ilk"
del "..\bin\debug\*.pdb"

del "..\bin\release\*.exp"
del "..\bin\release\*.map"
del "..\bin\release\*.pdb"
