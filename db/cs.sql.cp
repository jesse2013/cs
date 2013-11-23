create table users(id integer primary key, name text, passwd text, online text, fd integer);
insert into users(name, passwd, online, fd) values('troy', 'troy', 'off', -1);
insert into users(name, passwd, online, fd) values('zyy', 'zyy', 'off', -1);
insert into users(name, passwd, online, fd) values('tfy', 'tfy', 'off', -1);
insert into users(name, passwd, online, fd) values('jdy', 'jdy', 'off', -1);
create table troy(id integer primary key, name text, log_type integer);
create table zyy(id integer primary key, name text, log_type integer);
create table tfy(id integer primary key, name text, log_type integer);
create table jdy(id integer primary key, name text, log_type integer);
