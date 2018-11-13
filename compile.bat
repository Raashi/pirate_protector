g++ maker.cpp utils.cpp -o maker.exe
g++ signer.cpp structure.cpp utils.cpp -o signer.exe
g++ main.cpp structure.cpp utils.cpp -o a.exe
maker.exe a.exe signer.exe
del maker.exe
del signer.exe
