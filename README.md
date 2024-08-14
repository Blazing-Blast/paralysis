# Probability Calculator for Austin

## The competition

On the 10th of August 2024, Austin from ShoddyCast showed his Python code that took eight days to run a basic algorithm:

    Roll 231 four-sided dice and record the number of times it landed on 1.
    Repeat this 1 billion times, and output the largest total.

He challenged his viewers to make it faster. This is my submission to that competition.

## My time

Running the Linux version multi-threaded on an Intel Core i9 9900K can be done in 8.869 seconds.

The single threaded version achieves 1 minute 16 seconds.

## Implementation details

All variables are declared outside of the loops. This is to prevent the need for stack frames inside the loops.
(Which would take extra CPU instructions to create and get rid of)

I use a Mersenne Twister with a 19937-bit state and 64-bit code
because it is faster than all other random number generators that I've found, while having a uniform distribution.

Instead of rolling a die from 1 to 4, I roll it from 0 to 3, this makes the result fit into two bits.
Because a 4-sided die can be represented in 2 bits. So you can get 16 dice-rolls out of a 32-bit integer.

The loops from line 25 to 29 and 32 to 36 take a generated random number and split it up into pairs bits, this reduces the amount of random numbers needed.

Because 1 billion is not divisible by 16, the second loop is there to roll the 7 remaining dice.

Bigger numbers take longer to generate, but can be used to roll more dice.
I have found that 32 bits have the best tradeoff between these two.
This is what the many `ROLL;`s are for.
These are not in a loop (anymore) because loops are more expensive than just writing it 16 times.

Because all of these calculations (except tracking the maximum) are independent, you can do multiple at the same time.
This means you can multi-thread it. The code takes the amount of threads you have, and equally divides the work between them.
The leftover work is done by the main thread while it waits for the other 16-or-so threads to finish.
Once all are done, the biggest value between them gets outputted to the console.

## The Python version

I have first written a version in Python, extrapolating out from the 1 million result, it would take around 4 hours.

## Compiling, running, and timing

### Windows users

First, follow [this guide](https://www.msys2.org/).

Keep the MSYS2 application open. You will need it later.
If you accidentally closed it, make sure to open the UCRT64 variant.
(Or UCRT32 if you still have a 32-bit computer.)

Then, if you have already cloned this repo, use option 2.
If you have had the foresight to read this far ahead, use option 1.

Once you have the source code inside MSYS2,
open MSYS2 and enter the directory with

    cd paralysis

Finally, compile, run, and time the code by running

    ./run-windows.sh

#### Option 1

In the MSYS2 terminal, run

    pacman -S git

and then clone the repo from the terminal using

    git clone https://github.com/Blazing-Blast/paralysis

#### Option 2

Copy/move the folder with the code into `C:\msys64\home\USERNAME\`
or another directory if you've installed MSYS2 somewhere else.

### Linux users

clone the repo, enter the directory, and run

    ./run-linux.sh
