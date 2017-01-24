### Create a directory for the project:
```bash
mkdir epiClassifier
cd epiClassifier
```
Copy epiClassifierForLinux.zip in this directory and unzip it:

```bash
unzip epiClassifierForLinux.zip
```

If unzip is not installed already, install it

```bash
apt-get install zip
apt-get install unzip
```

### Install NSPR

Create a directory:

```bash
mkdir pr
cd pr
```

From [NSPR build instructions]:

If cvs is not installed already, install it, along with various useful things

<pre class=“code”># apt-get install build-essential cvs
</pre>

Check out the source tree from CVS

<pre class=“code”># cvs -q -d :pserver:anonymous@cvs-mirror.mozilla.org:/cvsroot co -r NSPR_4_8_RTM mozilla/nsprpub
</pre>

Create a build directory:

<pre class=“code”># mkdir target.debug
cd target.debug

</pre>

If gcc is not installed already, install it

<pre class=“code”># apt-get install gcc
</pre>

Run the configure script (this is the place to specify some options if needed)

On a dual 32-bit/64-bit platform, NSPR build generates a 32-bit build by default. To generate a 64-bit build, specify the --enable-64bit configure option.

<pre class=“code”># …/mozilla/nsprpub/configure --enable-64bit
</pre>

On 32 bits platforms, just type

<pre class=“code”># …/mozilla/nsprpub/configure
</pre>

Build the libraries

<pre class=“code”># make
</pre>
Build epiClassifier

Come back to directory:

<pre class=“code”># cd …/…
</pre>

And build epiClassifier:

<pre class=“code”># make
</pre>

… or, in order to send output to a file in order to debug:

<pre class=“code”># make &> results.txt
</pre>
Launch

To start epiClassifier, it is enough to do:

<pre class=“code”># ./epiClassifier
</pre>

However, when loging out, the task will automatically be killed because its parent process terminates. To have epiClassifier still run, it is necessary to use a special method of starting the process, for example nohup. With a remote connection, a commonly used method is to run the desired program with ‘screen’ (‘screen programname’). Then the program seems to run normally (but it’s actually running on screen), and when you want to exit, you can press CTRL+A, then D, to detach (“put the process in the back”, make it disappear from the foreground but still have it run there on ‘screen’), and just exit - and the process still runs. Later when you come back (maybe re-login remotely) you can run ‘screen -d -r’ which re-attaches (and possibly de-attaches the process first, if needed) to the process and you get it back to your screen.

<pre class=“code”># screen -S classif ./epiClassifier

CTRL+A, then D
</pre>
Mysql setup

So far, epiClassifier opens a database connction at startup. Mysql must be configured so that this connection doesn’t get timed out.

Edit my.cnf (with Debian distro, it is located in /etc/mysql directory)

<pre class=“code”>max_connections = 1500
max_user_connections = 600
wait_timeout = 259200
</pre>
Usual errors
cannot find -lmysqlclient

Probably comes from missing libmysqlclient-dev package; install it:

<pre class=“code”># apt-get install libmysqlclient-dev
</pre>

or, if using MariaDb

<pre class=“code”># apt-get install libmariadbclient-dev
</pre>
cannot find stubs-32.h

On a dual 32-bit/64-bit platform, NSPR build generates a 32-bit build by default. To generate a 64-bit build, specify the --enable-64bit configure option.

<pre class=“code”># …/mozilla/nsprpub/configure --enable-64bit
</pre>
error while loading shared libraries: libnspr4.so: cannot open shared object file: No such file or directory

Copy libnspr4.so to common libs directory (usually /usr/lib).

<pre class=“code”># cp ./pr/target.debug/dist/lib/libnspr4.so /usr/lib/libnspr4.so
</pre>

[NSPR build instructions]: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSPR/NSPR_build_instructions
