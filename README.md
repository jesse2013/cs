cs
==============================================
communication server

use c language on linux

cc
==============================================
communication client

use c language on linux

use c++ language on win7

need
==============================================
yum install sqlite.i686

how to use?
==============================================
root@cs# make

root@cs# ./cs       /* default port: 8888 */

root@cs# ./cs/cc

/* request type that client send to server */

:0:name:passwd:20131117100404           //register

:1:name:passwd:20131117100404           //login

:2:20131117100404                       //view all user

:3:ivy:20131117100404:hello world.      //send content to ivy

:4:ivy                                  //view log with ivy
