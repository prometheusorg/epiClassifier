These instructions were tested on Debian Jessie.
### Dependencies
```bash
apt-get install zip
apt-get install unzip
apt-get install build-essential
apt-get install mercurial
apt-get install build-essential
```

### Create a directory for the project:
```bash
mkdir epiClassifier
cd epiClassifier
```
Copy epiClassifierForLinux.zip in this directory and unzip it:

```bash
unzip epiClassifierForLinux.zip
```
### NSPR

From [NSPR build instructions]:

Check out the source tree from Mercurial

```bash
hg clone https://hg.mozilla.org/projects/nspr
```
Rename nspr directory to pr
```bash
mv nspr pr
```
Create a build directory:
```bash
mkdir target.debug
cd target.debug
```

Run the configure script (this is the place to specify some options if needed)

On a dual 32-bit/64-bit platform, NSPR build generates a 32-bit build by default. To generate a 64-bit build, specify the --enable-64bit configure option.

```bash
…/configure --disable-debug --enable-optimize --enable-64bit
```

On 32 bits platforms, just type

```bash
…/configure --disable-debug --enable-optimize
```

Build the libraries

```bash
make -j4
```

Build epiClassifier

Come back to directory:
```bash
cd …/…
```

And build epiClassifier:

```bash
make -j4
```

… or, in order to send output to a file in order to debug:
```bash
make &> results.txt
```

### Launch

To start epiClassifier, it is enough to do:

```bash
./epiClassifier
```

However, when loging out, the task will automatically be killed because its parent process terminates. To have epiClassifier still run, it is necessary to use a special method of starting the process, for example nohup. With a remote connection, a commonly used method is to run the desired program with ‘screen’ (‘screen programname’). Then the program seems to run normally (but it’s actually running on screen), and when you want to exit, you can press CTRL+A, then D, to detach (“put the process in the back”, make it disappear from the foreground but still have it run there on ‘screen’), and just exit - and the process still runs. Later when you come back (maybe re-login remotely) you can run ‘screen -d -r’ which re-attaches (and possibly de-attaches the process first, if needed) to the process and you get it back to your screen.

```bash
screen -S classif ./epiClassifier
```
CTRL+A, then D

Mysql setup

So far, epiClassifier opens a database connction at startup. Mysql must be configured so that this connection doesn’t get timed out.

Edit my.cnf (with Debian distro, it is located in /etc/mysql directory)

```bash
max_connections = 1500
max_user_connections = 600
wait_timeout = 259200
```bash

### Usual errors

## cannot find -lmysqlclient

Probably comes from missing libmysqlclient-dev package; install it:

```bash
apt-get install libmysqlclient-dev
```

or, if using MariaDb

```bash
apt-get install libmariadbclient-dev
```
cannot find stubs-32.h

On a dual 32-bit/64-bit platform, NSPR build generates a 32-bit build by default. To generate a 64-bit build, specify the --enable-64bit configure option.

```bash
../configure --enable-64bit
```
## error while loading shared libraries: libnspr4.so: cannot open shared object file: No such file or directory

Copy libnspr4.so to common libs directory (usually /usr/lib).

```bash
cp ./pr/target.debug/dist/lib/libnspr4.so /usr/lib/libnspr4.so
```

### References
[NSPR build instructions]: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSPR/NSPR_build_instructions
