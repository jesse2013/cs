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


server运行环境
==============================================
CentOS release 6.4

download: http://mirrors.163.com/centos/6.4/isos/


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
linux / windows / android / ios


通信协议  client的请求 及 server的回应
==============================================
client的请求

ok 注册用户		:0:troy:troy:::

销毁用户		:1:::::

ok 登陆			:2:troy:troy:::

退出			:3:::::

ok 查看所有用户	:4:::::

ok 添加好友		:5:troy:_:ivy::

ok 删除好友		:6:troy:_:ivy::

ok 与好友聊天	:7:troy:_:zyy:nihao,zyy:20131117100404

ok查看聊天记录	:8:troy:_:zyy::

删除聊天记录	:9:troy:_:ivy::


server的回应

00	未知请求类型

10	成功(转为登录状态)

11	表操作失败

12	用户名已存在

13	此用户不存在

......


数据库(cs.db)设计
==============================================
1.全部用户名 及 是否在线状态 的表

create table users(id integer primary key, name text, passwd text, online text, fd integer);

insert into users(name, passwd, online, fd) values('troy', 'troy', 'off', -1);

insert into users(name, passwd, online, fd) values('zyy', 'zyy', 'off', -1);

2.表名为用户名 此用户的所有好友

create table troy(id integer primary key, name text, log_type integer);

insert into troy(name, log_type) values('zyy', 0);

insert into zyy(name, log_type) values('troy', 1);

3.表名为用户名-好友用户名 存放聊天记录

create table troy_zyy(id integer primary key, name text, content text, datetime text);

insert into troy_zyy(name, content, datetime) values('troy', 'hello,ivy', '20131117100404');

insert into troy_zyy(name, content, datetime) values('zyy', 'hello,troy', '20131117100504');


表操作
==============================================
select troy.name, users.online from troy,users where troy.name=users.name;	//多表查询

select count(*) from sqlite_master where type='table' and name='user';		//表是否存在

create table users(id integer primary key, name text, passwd text, online text);

insert into users(id, name, passwd, online) values(1, 'root', '123', 'off');

select * from user where name='troy' and passwd='troy';

update employee set age=25, name='dongqiang' where name='dq';

delete from employee where id=5 or name="cj";

drop table troy_ivy;		//删除表


术语
==============================================
cs	communication server

cc	communication client

db	database


need
==============================================
yum install sqlite.i686


how to use?
==============================================
root@cs# make sql	/*初始化数据库*/

root@cs# make

root@cs# ./cs       /* start cs, default port: 8888 */

[11.00:35:38.723]:cs->src/cs_server.c->main:00285 --> cs start 0.0.0.0 8888

root@cs# ./cc/cc

[11.00:51:13.362]:cc->cc_client.c->main:00071 --> connect 127.0.0.1 at PORT 8888 success.

[11.00:51:13.363]:cc->cc_client.c->receive_routine:00010 --> *****receive_routine() Enter*****

:0:guest:guest:::		/* 注册新用户 */