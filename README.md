��Ƴ���
==============================================
ר�������ҵ���������칤��,��󻯷���Ա����ͨ�ͽ���.

1.���ݰ�ȫ,����ֻ����������.

2.���͹�˾�Ի�������������,�������Ϳ�������ʹ��,֧����������.

3.���Ի�����,��˾����������Ϣ�����пͻ���,��¼��Ĭ������ͬ�¶�����ĺ���,�ȵ�.


��ƽ׶�
==============================================
��һ�׶�

1.server ��������,��linux��ʹ��c����

2.linux �����пͻ��˿���

3.windows ͼ�οͻ��˿��� (��ʾ �Է���������)

4.server ��Ϣȫ������

�ڶ��׶�

1.server����cs.conf�ļ�����

2.ʵ���ļ�ת��

3.�ͻ���ʵ����Ļ��ͼ

4.Ⱥ�鹦��ʵ��

5.linux ͼ�οͻ��˿���

�����׶�

1.android ͼ�οͻ��˿���

2.ios ͼ�οͻ��˿���


server����
==============================================
1.�û���֤

2.���ѹ�ϵ�ж�

3.����client�ĺϷ�����

4.ͬһ�������ж�

5.������Ϣת��(֧�ֶԷ�����,��������)

6.�ļ�ת��(֧�ֶԷ�����,��������)

7.���͹���(֧�ֶԷ�����,��������)


serverģ��
==============================================
0.���������ļ� cs.conf

1.�����¼���Ӧ,ʹ�� ������ + select

2.�¼������߼�

3.���ݿ����,ʹ�� sqlite


server���л���
==============================================
CentOS release 6.4

download: http://mirrors.163.com/centos/6.4/isos/


client����
==============================================
1.ע��

2.��¼

3.�鿴�����û�������״̬(֧�ַ���)

4.��Ӻ���

5.ɾ������

6.����������Ϣ

7.����������Ϣ

8.�鿴�����¼

9.ɾ�������¼

10.���չ�����Ϣ

11.�����ļ�

12.��Ļ��ͼ

13.Ⱥ�齨


client֧��ƽ̨
==============================================
linux / windows / android / ios


ͨ��Э��  client������ �� server�Ļ�Ӧ
==============================================
client������

ok ע���û�		:0:troy:troy:::

ok �����û�		:1:troy:troy:::

ok ��½			:2:troy:troy:::

ok �˳�			:3:troy::::

ok �鿴�����û�	:4:::::

ok ��Ӻ���		:5:troy:_:ivy::

ok ɾ������		:6:troy:_:ivy::

ok ���������	:7:troy:_:zyy:nihao,zyy:20131117100404 (֧��������Ϣ)

ok �鿴�����¼	:8:troy:_:zyy::

ok ɾ�������¼	:9:troy:_:zyy::

ok �޸�����     :10:troy:troy:_:������:

ok �ύ�û���Ϣ :11:troy:_:_:����&�ֻ���&�ֻ���:

ok ��ѯ�û���Ϣ	:12:troy:_:zyy::


server�Ļ�Ӧ

00	δ֪��������

10	�ɹ�(תΪ��¼״̬)

11	�����ʧ��

12	�û����Ѵ���

13	���û�������

......


���ݿ�(cs.db)���
==============================================
1.ȫ���û��� �� �Ƿ�����״̬ �ı�

create table users(id integer primary key, name text, passwd text, fd integer, email text, phone text, tel text);

insert into users(name, passwd, fd, email, phone, tel) values('troy', 'troy', -1, 'troy@smit.com', '13263102329', '6666');

insert into users(name, passwd, fd, email, phone, tel) values('zyy', 'zyy', -1, 'zyy@smit.com', '13263102328', '7777');

2.����Ϊ�û��� ���û������к���

create table troy(id integer primary key, name text, log_type integer);

insert into troy(name, log_type) values('zyy', 0);

insert into zyy(name, log_type) values('troy', 1);

3.����Ϊ�û���-�����û��� ��������¼

create table troy_zyy(id integer primary key, name text, content text, datetime text);

insert into troy_zyy(name, content, datetime) values('troy', 'hello,ivy', '20131117100404');

insert into troy_zyy(name, content, datetime) values('zyy', 'hello,troy', '20131117100504');


�����
==============================================
select troy.name, users.online from troy,users where troy.name=users.name;	//����ѯ

select count(*) from sqlite_master where type='table' and name='user';		//���Ƿ����

create table users(id integer primary key, name text, passwd text, online text);

insert into users(id, name, passwd, online) values(1, 'root', '123', 'off');

select * from user where name='troy' and passwd='troy';

update employee set age=25, name='dongqiang' where name='dq';

delete from employee where id=5 or name="cj";

drop table troy_ivy;		//ɾ����


����
==============================================
cs	communication server

cc	communication client

db	database


need
==============================================
yum install sqlite.i686


how to use?
==============================================
root@cs# make sql	/* ��ʼ�����ݿ� */

root@cs# make

root@cs# ./cs       /* start cs, default port: 8888 */

[11.00:35:38.723]:cs->src/cs_server.c->main:00285 --> cs start 0.0.0.0 8888

root@cs# ./cc/cc

[11.00:51:13.362]:cc->cc_client.c->main:00071 --> connect 127.0.0.1 at PORT 8888 success.

[11.00:51:13.363]:cc->cc_client.c->receive_routine:00010 --> *****receive_routine() Enter*****

:0:guest:guest:::		/* ע�����û� */
