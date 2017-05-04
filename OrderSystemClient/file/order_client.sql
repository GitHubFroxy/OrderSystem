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

INSERT INTO menu_info VALUES('17','荷兰豆',20,'');
INSERT INTO menu_info VALUES('18','蒜蓉扇贝',33,'');
INSERT INTO menu_info VALUES('19','煎薯角',18,'');
INSERT INTO menu_info VALUES('20','鸡蛋煎饼',8,'');
INSERT INTO menu_info VALUES('21','孜然土豆',13,'');
INSERT INTO menu_info VALUES('22','排骨烧胡萝卜莴笋',22,'');
INSERT INTO menu_info VALUES('23','八寸菠菜戚风蛋糕',25,'');
INSERT INTO menu_info VALUES('24','春笋五花肉',33,'');
INSERT INTO menu_info VALUES('25','热干面',12,'');
INSERT INTO menu_info VALUES('26','炸鸡排',22,'');
INSERT INTO menu_info VALUES('27','美味香菇培根披萨',38,'');
		

---创建顾客订单表,用户简单的统计销售额(订单流水号，名称，价格，日期)
CREATE TABLE order_list(order_id VARCHAR(20),
						order_name VARCHAR(50),
						order_price DOUBLE,
						order_data VARCHAR
						);
---插入数据
INSERT INTO order_list VALUES();

---创建菜单订单临时表
CREATE TABLE temp_menu(menu_name varchar(20),
					   menu_num double,
					   menu_price double
					   )
						
						
