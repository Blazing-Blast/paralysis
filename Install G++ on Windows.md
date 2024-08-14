# Installing g++

First, follow [this guide](https://www.msys2.org/).
During steps 6 and 8, replace `gcc` with `g++`.

Assuming you used `C:\msys64\` in step 3,
add `C:\msys64\ucrt64\bin\` to PATH.

Open up a normal Windows command prompt (not an MSYS2 one),
and type `g++ --version`.
If this does not return `g++.exe (Rev2, Built by MSYS2 project) {VERSION}`,
file an issue and I will assist in further installation.
