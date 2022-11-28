echo "Building"
cd "E:\Sclone 2.0\GeneratedOutput"
g++.exe -Wall -DSFML_STATIC -O2 -Wmissing-include-dirs -pedantic-errors -Wfatal-errors -Wextra -Wall -std=c++17 -Werror=return-type -IE:\SFML-2.5.1\include -c main.cpp -o obj\Release\main.o
g++.exe -LE:\SFML-2.5.1\Build\lib -o bin\Release\GeneratedOutput.exe obj\Release\main.o  -s  -lsfml-graphics -lsfml-window -lsfml-system
echo "[Done] Building."
bin\Release\GeneratedOutput.exe
