==========================================
eLua development environment on mbed board
==========================================

1) Install lua5.1 into Ubuntu machine:
    * Visit 'http://packages.ubuntu.com/lucid/lua5.1' to download.
    $ which lua
    /usr/bin/lua
    $ lua -v
    Lua 5.1.5  Copyright (C) 1994-2012 Lua.org, PUC-Rio
 
    *IMPORTANT*
    Make sure that lua is NOT version 5.2. 5.2 can not success the following steps!

2) Check out: https://github.com/Shengliang/elua.git into Ubuntu local git repository:
    $ git clone https://github.com/Shengliang/elua
    $ cd elua
    $ git pull

3) Install LuaRocks (a development and management system for Lua modules):
    $ sudo apt-get install luarocks
    $ luarocks install luafilesystem
    $ luarocks install lpack
    $ luarocks install md5

4) Run build script from elua home directory on your Ubuntu
    $ sudo ./build.sh
    # Make sure that elua.bin is created
    $ ls -lth elua.bin

5) Flash elua.bin into your mbed board

6) Create 'HelloWorld.lua' for verification, and flash to mbed board
   $ cat HelloWorld.lua
   print("Hello World!!")

7) Setup USB/Serial console
   $ sudo minicom -D /dev/ttyACM0 
   Contents of mbed flash:
   //////////////////////////////////////////////////////////////////////////////
   eLua# ls

   /rom

   Total on /rom: 0 bytes

   /semi
   MBED.HTM                       341 bytes
   ELUA.BIN                       199472 bytes
   HELLOW~1.LUA                   22 bytes

   Total on /semi: 199835 bytes

    eLua#
    /////////////////////////////////////////////////////////////////////////////

  8) Run "Hello World" program
    eLua#  lua /semi/HELOW-1.LUA
    Press CTRL+Z to exit Lua
    Hello World!!
    eLua#


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Objective: Study elua development environment

Check out: https://github.com/Shengliang/elua.git

Please use gitk to review those change logs. 
  - apt-get install gitk

- a new shell command in elua shell.
- a new API that can be called from elua.

This script requires some well-known Lua libraries to run.
To install them on Ubuntu/Debian, go (as root):
apt-get install luarocks
luarocks install luafilesystem
luarocks install lpack
luarocks install md5
 
Then, type "./build.sh" to compile.

Study Notes:
How to create a cscope?
   find . -name "*.[chS]" > cscope.files
   vi cscope.files
   cscope

How to create & appy a git patch?
 git ls-remote https://github.com/shengliang/e4357.git 

# git: create a patch from top cl
   git show [hashID]
   git format-patch HEAD~

   # extract 2 topmost commits
   git format-patch -2

# apply a patch
   git am <...patch>

# add files
   git diff
   git status
   git add build.sh
   git add src/shell/shell_mem.c
   git commit -a -m 'shell: add md'
   git commit -a -m 'lua: add md'
   git commit -a --amend 

# checkin local branch to remote branches
   git remote -v
   git push origin local_branch_name:remote_branch_name
   git push origin master:master
   git push origin :remtoe_branch_name  //delete a remote branhc

# change remote url
   git remote set-url origin git://new.url.here
   git push origin master:master
