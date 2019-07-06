create table stock_date_601857
(
    id int primary key auto_increment,
    code int default 0,
    name varchar(16) default '',
    today_start float default 0,
    today_peak float default 0,
    today_low float default 0,
    yesterday_end float default 0,
    trust_rate float default 0,
    exchange_rate float default 0,
    range_rate float default 0,
    deal_volume float default 0,
    date varchar(32) not null
)
