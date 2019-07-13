create table stock_history_sh601857
(
    id int primary key auto_increment,
    code varchar(16) not null,
    name varchar(16) default '',
    yesterday_end float default 0,
    today_start float default 0,
    today_close float default 0,
    today_max float default 0,
    today_min float default 0,
    today_deal_volume float default 0,
    today_deal_CNY float default 0,
    average_price float default 0,
    today_change float default 0,
    today_change_rate varchar(8) default '0',
    trust_rate varchar(8) default '0',
    exchange_rate varchar(8) default '0',
    range_rate varchar(8) default '0',
    date varchar(32) not null,
    unique(code, date)
)ENGINE=InnoDB  DEFAULT CHARSET=utf8;

