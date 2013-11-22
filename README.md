设计初衷
==============================================
专门针对企业开发的聊天工具,最大化方便员工沟通和交流.

1.数据安全,数据只在内网传送.

2.降低公司对互联网的依赖性,有内网就可以正常使用,支持外网接入.

3.个性化定制,公司可以推送消息到所有客户端,登录后默认所有同事都是你的好友,等等.


设计阶段
==============================================

第一阶段

1.server 正常工作,在linux下使用c开发

2.linux 命令行客户端开发

3.linux 图形客户端开发

第二阶段

1.server依据cs.conf文件配置

3.实现文件转发

4.windows 图形客户端开发

5.客户端实现屏幕截图

第三阶段

1.android 图形客户端开发

2.ios 图形客户端开发


server功能
==============================================

1.用户验证

2.好友关系判断

3.处理client的合法请求

4.同一局域网判断

5.聊天消息转发(支持对方离线,上线提醒)

6.文件转发(支持对方离线,上线提醒)

7.推送公告(支持对方离线,上线提醒)


server模块
==============================================

0.解析配置文件 cs.conf

1.网络事件响应,使用 单进程 + select

2.事件处理逻辑

3.数据库操作,使用 sqlite

●●●server运行环境

CentOS release 6.4


client功能
==============================================

1.注册

2.登录

3.查看所有用户及在线状态(支持分组)

4.添加好友

5.删除好友

6.接收聊天消息

7.发送聊天消息

8.查看聊天记录

9.删除聊天记录

10.接收公告消息

11.发送文件

12.屏幕截图

13.群组建


client支持平台
==============================================
linux

windows

android

ios


通信协议  client的请求 及 server的回应
==============================================

:request_type:username:passwd:whoname:content:request_time

client向server发送的请求类型

:0:name:passwd:::				//注册

返回:成功(转为登录状态)/失败(原因)

:1:troy:troy:::20131117100404

:1:name:passwd:public ip:private ip:20131117100404	//登录

返回:好友列表

:2:::::						//查看所有用户

返回:用户列表

:3:::ivy::20131117100404			//添加好友

返回:成功/失败

:4:::ivy::20131117100404			//删除好友

返回:成功/失败

:5:::ivy:hello world.:20131117100404		//发送聊天消息

推送消息给对方 更新自己和对方的log文件

返回:成功/失败

:6:::ivy::					//查看聊天记录

返回:当前的聊天记录

:7:::ivy::					//删除聊天记录

返回:成功/失败

:8:::ivy:filename:20131117100404		//发送文件

返回:成功/失败


数据库(cs.db)设计
==============================================

1.全部用户名 及 是否在线状态 的表

create table users(id integer primary key, name text, passwd text, online text);

insert into users(id, name, passwd, online) values(1, 'root', '123', 'off');

insert into users(id, name, passwd, online) values(2, 'troy', '456' 'off');

2.表名为用户名 此用户的所有好友

create table troy(id integer primary key, name text, log_state int);

insert into troy(id, name, online) values(1, 'ivy', 0);

insert into troy(id, name, online) values(2, 'cy', 1);

insert into troy(id, name, online) values(3, 'jdy', 2);

3.表名为用户名-好友用户名 存放聊天记录

create table troy_ivy(id integer primary key, content text, datetime text);

insert into troy_ivy(id, content, datetime) values(1, 'hello ivy', '20131117100404');

insert into troy_ivy(id, content, datetime) values(2, 'hello troy', '20131117100504');


表操作
==============================================

select count(*) from sqlite_master where type='table' and name='user';		//表是否存在

create table users(id integer primary key, name text, passwd text, online text);

insert into users(id, name, passwd, online) values(1, 'root', '123', 'off');

select * from user where name='troy' and passwd='troy';

update employee set age=25, name='dongqiang' where name='dq';

delete from employee where id=5 or name="cj";



cs
==============================================
communication server

use c language on linux

need
==============================================
yum install sqlite.i686

how to use?
==============================================
root@cs# make

root@cs# ./cs       /* default port: 8888 */

root@cs# ./cs/cc
