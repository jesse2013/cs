create table users(id integer primary key, name text, passwd text, fd integer);
insert into users(name, passwd, fd) values('troy', 'troy', -1);
insert into users(name, passwd, fd) values('zyy', 'zyy', -1);
create table troy(id integer primary key, name text, log_type integer);
insert into troy(name, log_type) values('zyy', 0);
create table zyy(id integer primary key, name text, log_type integer);
insert into zyy(name, log_type) values('troy', 1);
create table troy_zyy(id integer primary key, name text, content text, datetime text);
