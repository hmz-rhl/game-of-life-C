# game-of-life-C
This is a C programmed software which simulates the game of life.

## Requirements
this software use the ncurses library, so it is need to be previously installed by typing :
```
sudo apt install libncurses5-dev libncurses5
```

## how to compile

Just download this repository. In order to just compile it, on a terminal you have to run this command where the files are downloaded :

```
sudo make
```

then you can run it :

```
./GOL.exe
```

## installation
If you want to install it on your kernel, just type run these commands :
```
sudo make
sudo make install
```
After that, you will be able to run the command outside this directory, just run :
```
cd
GOL
```
## Have fun
Try what ever configuration you want, you can also modify the size of the screen by defining your own macro "SIZE", then the screen will be SIZE*SIZE (40*40 by default)