---待做菜单信息表
CREATE TABLE menu_info(menu_id varchar(10) primary key,
						table_id varchar(10),
						menu_name varchar(20),
						menu_state varchar(20) default '未做',
						menu_remark varchar(40)
						);