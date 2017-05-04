---����������̨��Ϣ��(���ӱ�ţ����ԣ���ע)
CREATE TABLE table_info(table_id VARCHAR(10) PRIMARY KEY,
					    table_property VARCHAR(50),
						table_remark VARCHAR(100)
						);
---��������
INSERT INTO table_info VALUES('001',"��ͨ",'');
INSERT INTO table_info VALUES('002',"��ͨ",'');
INSERT INTO table_info VALUES('003',"��ͨ",'');
INSERT INTO table_info VALUES('004',"��ͨ",'');
INSERT INTO table_info VALUES('005',"��ͨ",'');
INSERT INTO table_info VALUES('006',"��ͨ",'');
INSERT INTO table_info VALUES('007',"��ͨ",'');
INSERT INTO table_info VALUES('008',"��ͨ",'');
INSERT INTO table_info VALUES('009',"��ͨ",'');
INSERT INTO table_info VALUES('010',"��ͨ",'');
INSERT INTO table_info VALUES('021',"����",'');
INSERT INTO table_info VALUES('022',"����",'');
INSERT INTO table_info VALUES('023',"����",'');
INSERT INTO table_info VALUES('024',"����",'');
INSERT INTO table_info VALUES('025',"����",'');
INSERT INTO table_info VALUES('026',"����",'');
INSERT INTO table_info VALUES('027',"����",'');
INSERT INTO table_info VALUES('028',"����",'');
INSERT INTO table_info VALUES('029',"����",'');
INSERT INTO table_info VALUES('030',"����",'');


---�����˵���Ϣ��(�˵���ţ����ƣ��۸񣬱�ע)
CREATE TABLE menu_info( menu_id VARCHAR(10) PRIMARY KEY,
						menu_name VARCHAR(50),
						menu_price DOUBLE,
						menu_remark VARCHAR(100)
						);
---��������
INSERT INTO menu_info VALUES('1','������˿',12,'');
INSERT INTO menu_info VALUES('2','��������',12,'');
INSERT INTO menu_info VALUES('3','�ع���',12,'');
INSERT INTO menu_info VALUES('4','���Ŷ���',10,'');
INSERT INTO menu_info VALUES('5','�����Ź�',15,'');
INSERT INTO menu_info VALUES('6','ˮ����',16,'');
INSERT INTO menu_info VALUES('7','ëѪ��',18,'');
INSERT INTO menu_info VALUES('8','��ĩ����',10,'');
INSERT INTO menu_info VALUES('9','����ƹ�',9,'');
INSERT INTO menu_info VALUES('10','���Զ���',10,'');
INSERT INTO menu_info VALUES('11','����Ϻ',18,'');
INSERT INTO menu_info VALUES('12','��������',22,'');
INSERT INTO menu_info VALUES('13','����С��Ϻ',28,'');
INSERT INTO menu_info VALUES('14','��ǹ��ɫ��',40,'');
INSERT INTO menu_info VALUES('15','�ջƻ���',35,'');
INSERT INTO menu_info VALUES('16','�ɹ�ǧҶ����',32,'');
		

---�����˿Ͷ�����,�û��򵥵�ͳ�����۶�(������ˮ�ţ����ƣ��۸�����)
CREATE TABLE order_list(order_id VARCHAR(20),
						order_name VARCHAR(50),
						order_price DOUBLE,
						order_data VARCHAR
						);
---��������
INSERT INTO order_list VALUES();


---
CREATE TABLE temp_order( table_id varchar(10),
                          menu_name varchar(20),                          
                          menu_price double,                          
                          menu_num double,   
                          menu_remark varchar(100),                          
                          FOREIGN KEY (table_id)  references table_info(table_id) 
                          )  

CREATE TABLE temp_table( table_id varchar(10));

---�û���ɫ��
CREATE TABLE user_role(role_id int primary key,
						role_name varchar(20)
						);
INSERT INTO user_role VALUES(0,'����Ա');


---�����û���Ϣ��
CREATE TABLE user_info(user_id int primary key,
						user_name varchar(10),
						user_pwd varchar(20),
						role_id int,
						foreign key (role_id) references user_role(role_id)
						);
INSERT INTO user_info VALUES('1','admin','admin',0);

CREATE VIEW  v_userInfo AS
SELECT user_id,user_name,user_pwd,user_role.[role_name] FROM user_info,user_role
       WHERE user_info.[role_id] = user_role.[role_id]
	   
---�������Ѽ�¼��
CREATE TABLE bill_data( bill_id varchar(20) primary key,
						table_id varchar(10),
						bill_price double,
						bill_date varchar(20)
						);

						
						
