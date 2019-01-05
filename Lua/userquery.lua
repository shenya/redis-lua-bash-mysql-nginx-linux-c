local cjson = require "cjson"
local redis = require "resty.redis"
local mysql = require "resty.mysql"


local red = redis:new()

local db,err = mysql:new()
if not db then
    ngx.say("failed to init mysql: ", err)
    return
end

db:set_timeout(1000)

local ok,err,errno,sqlstate=db:connect{
host = "127.0.0.1",
port = 3306,
database="test",
user="root",
password="sgy2017",
max_package_size=1024
}


if not ok then
ngx.say("failed to connect: ", err, ": ", errno, " ", sqlstate)
return
end


red:set_timeout(1000)

local ok, err = red:connect("127.0.0.1", 6379)
if not ok then

ngx.say("failed to connect: ", err)
return
end


ngx.say("uri: ", ngx.var.request_uri, "  name: ", ngx.var.arg_name)

if ngx.var.arg_name ~= nil then

    local result = red:hgetall(ngx.var.arg_name)

    if next(result) == nil then

    ngx.say("user: [", ngx.var.arg_name, "] not exist in redis, check it from db", "<br/>");
    sql="select id,name,query_times,create_timestamp,last_access_timestamp from student where name='"..ngx.var.arg_name.."'"
    ngx.say("sql", sql)
    res,err,errno,sqlstate = db:query(sql)
    if not res then
    ngx.say("bad result: ", err, ": ", errno, ": ", sqlstate, ".")
    return
    end

   ngx.say("hello", type(res))
    ngx.say("name:", type(res),res[1].id);
        ngx.say("get result from db: ", cjson.encode(res))

        ngx.say("add user: [", ngx.var.arg_name, "] it into redis");
       red:hmset(ngx.var.arg_name, "id", res[1].id, "query_times", res[1].query_times, "create_timestamp", res[1].create_timestamp, "last_access_timestamp", res[1].last_access_timestamp)

        sql="update student set query_times=query_times+1 where id="..res[1].id
        ngx.say(sql);
	res,err,errno,sqlstate = db:query(sql)
	if not res then
	ngx.say("update db result: ", err, ": ", errno, ": ", sqlstate, ".")
	return
        end
    else
        local count = 0;
        ngx.say(result[4])
        if result[4] ~= nil then
            count=result[4]
        end

        count = count + 1;

        red:hmset(ngx.var.arg_name, "last_access_timestamp", os.time(), "query_times", count)
        local result = red:hgetall(ngx.var.arg_name)

        ngx.say("get result from redis: ", cjson.encode(result))
        sql="update student set query_times=query_times+1 where id="..result[2]
        ngx.say(sql)
	res,err,errno,sqlstate = db:query(sql)
	if not res then
	ngx.say("update db result: ", err, ": ", errno, ": ", sqlstate, ".")
	return
        end
    end


end

red:close()
db:close()
