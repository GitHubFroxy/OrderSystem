---创建餐厅桌台信息表(桌子编号，属性，备注)
CREATE TABLE table_info(table_id VARCHAR(10) PRIMARY KEY,
					    table_property VARCHAR(50),
						table_remark VARCHAR(100)
						);
---插入数据
INSERT INTO table_info VALUES('001',"普通",'');
INSERT INTO table_info VALUES('002',"普通",'');
INSERT INTO table_info VALUES('003',"普通",'');
INSERT INTO table_info VALUES('004',"普通",'');
INSERT INTO table_info VALUES('005',"普通",'');
INSERT INTO table_info VALUES('006',"普通",'');
INSERT INTO table_info VALUES('007',"普通",'');
INSERT INTO table_info VALUES('008',"普通",'');
INSERT INTO table_info VALUES('009',"普通",'');
INSERT INTO table_info VALUES('010',"普通",'');
INSERT INTO table_info VALUES('021',"包房",'');
INSERT INTO table_info VALUES('022',"包房",'');
INSERT INTO table_info VALUES('023',"包房",'');
INSERT INTO table_info VALUES('024',"包房",'');
INSERT INTO table_info VALUES('025',"包房",'');
INSERT INTO table_info VALUES('026',"包房",'');
INSERT INTO table_info VALUES('027',"包房",'');
INSERT INTO table_info VALUES('028',"包房",'');
INSERT INTO table_info VALUES('029',"包房",'');
INSERT INTO table_info VALUES('030',"包房",'');


---创建菜单信息表(菜单编号，名称，价格，备注)
CREATE TABLE menu_info( menu_id VARCHAR(10) PRIMARY KEY,
						menu_name VARCHAR(50),
						menu_price DOUBLE,
						menu_remark VARCHAR(100)
						);
---插入数据
INSERT INTO menu_info VALUES('1','鱼香肉丝',12,'');
INSERT INTO menu_info VALUES('2','宫保鸡丁',12,'');
INSERT INTO menu_info VALUES('3','回锅肉',12,'');
INSERT INTO menu_info VALUES('4','麻婆豆腐',10,'');
INSERT INTO menu_info VALUES('5','红烧排骨',15,'');
INSERT INTO menu_info VALUES('6','水煮鱼',16,'');
INSERT INTO menu_info VALUES('7','毛血旺',18,'');
INSERT INTO menu_info VALUES('8','肉末茄子',10,'');
INSERT INTO menu_info VALUES('9','凉拌黄瓜',9,'');
INSERT INTO menu_info VALUES('10','干煸豆角',10,'');
INSERT INTO menu_info VALUES('11','香辣虾',18,'');
INSERT INTO menu_info VALUES('12','海鲜披萨',22,'');
INSERT INTO menu_info VALUES('13','香辣小龙虾',28,'');
INSERT INTO menu_info VALUES('14','金枪鱼色拉',40,'');
INSERT INTO menu_info VALUES('15','烧黄花鱼',35,'');
INSERT INTO menu_info VALUES('16','干锅千叶豆腐',32,'');
		

---创建顾客订单表,用户简单的统计销售额(订单流水号，名称，价格，日期)
CREATE TABLE order_list(order_id VARCHAR(20),
						order_name VARCHAR(50),
						order_price DOUBLE,
						order_data VARCHAR
						);
---插入数据
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

---用户角色表
CREATE TABLE user_role(role_id int primary key,
						role_name varchar(20)
						);
INSERT INTO user_role VALUES(0,'管理员');


---创建用户信息表
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
	   
---创建消费记录表
CREATE TABLE bill_data( bill_id varchar(20) primary key,
						table_id varchar(10),
						bill_price double,
						bill_date varchar(20)
						);

						
						
