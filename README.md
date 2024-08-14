# My implementation

## My time

Running the Linux version single threaded on an Intel Core i9 9900K, I get between 1 minute 20 and 1 minute 16.

## Compiling and Running

### Windows users

First, follow [this guide](https://www.msys2.org/).

In the MSYS2 terminal, run

    pacman -S git

and then clone the repo from the terminal using

    git clone https://github.com/Blazing-Blast/paralysis

Enter the directory with

    cd paralysis

Finally, compile, run, and time the code by running

    ./run-windows.sh

### Linux users

Just run

    ./run-linux.sh
