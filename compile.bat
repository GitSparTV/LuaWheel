g++ -O2 -c -o luawheel64.o luawheel.cpp -L64 -llua51 -lLogitechSteeringWheel
g++ -O -shared -o luawheel.dll luawheel64.o -L64 -llua51 -lLogitechSteeringWheel
:: This is for 32bit
:: g++ -m32 -O2 -c -o luawheel32.o luawheel.cpp -L32 -llua51 -lLogitechSteeringWheel
:: g++ -m32 -O -shared -o 32/luawheel.dll luawheel32.o -L32 -llua51 -lLogitechSteeringWheel
pause