create table student
(
    id int primary key auto_increment,
    name varchar(32) default '',
    query_times int default 0,
    create_timestamp int default 0,
    last_access_timestamp timestamp default current_timestamp on update current_timestamp
);
