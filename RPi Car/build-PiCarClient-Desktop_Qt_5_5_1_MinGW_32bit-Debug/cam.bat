echo off
netcat-1.11\nc.exe -l -p %1 | mplayer\mplayer.exe -fps 60 -cache 2048 -