
# -*- coding:utf-8 -*-
import MySQLdb



#code=601857
#name='zgsy'
#today_start=6.87
#today_peak=6.7
#today_low=7.7
#yesterday_end=8.7
#trust_rate=9.7
#exchange_rate=10.1
#range_rate=22.3
#deal_volume=33.3
#date='20190706'


def stock_store_into_db(table, code, name, close_price, today_change, today_change_rate, today_start, today_peak, today_low, yesterday_end, trust_rate, exchange_rate, range_rate, deal_volume, date):

    db = MySQLdb.connect("localhost", "root", "szkbsgy2019", "stock_info", charset='utf8')

    cursor = db.cursor()

    print(type(code))
    #sql="""insert into stock_date_601857(code, name, today_start,today_peak,today_low, yesterday_end,trust_rate,
     #   exchange_rate, range_rate, deal_volume,date) values("%s","%s",%f,%f,%f,%f,%f,%f,%f,%f,"%s")"""%(code,name, today_start,today_peak,today_low,\
      #      yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)

    sql="""insert into %s(code, name, close_price, today_change, today_change_rate, today_start,today_peak,today_low, yesterday_end,trust_rate,
        exchange_rate, range_rate, deal_volume,date) values("%s","%s", "%s", "%s", "%s", "%s","%s","%s","%s","%s","%s","%s","%s","%s")"""%(table, code,name, close_price, today_change, today_change_rate, today_start,today_peak,today_low,\
            yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)
    print(sql)

    try:
        cursor.execute(sql)
        db.commit()
    except:
        print("error occ")
        db.rollback()

    db.close()


def stock_history_store_into_db(table, code, name, today_start, today_max, today_close, today_min, today_deal_volume, today_deal_CNY, average_price, date):

    db = MySQLdb.connect("localhost", "root", "szkbsgy2019", "stock_history_info", charset='utf8')
    try:
        print("hello")
    except:
        print("connect failed")
    cursor = db.cursor()

    print(type(code))
    #sql="""insert into stock_date_601857(code, name, today_start,today_peak,today_low, yesterday_end,trust_rate,
     #   exchange_rate, range_rate, deal_volume,date) values("%s","%s",%f,%f,%f,%f,%f,%f,%f,%f,"%s")"""%(code,name, today_start,today_peak,today_low,\
      #      yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)

    sql="""insert into %s(code, name, today_start, today_close, today_max, today_min, today_deal_volume, today_deal_CNY, average_price, date) \
    values("%s","%s", "%s", "%s", "%s", "%s","%s","%s","%s","%s")"""\
    %(table, code, name, today_start, today_close, today_max, today_min, today_deal_volume, today_deal_CNY, average_price, date)
    print(sql)

    try:
        cursor.execute(sql)
        db.commit()
    except:
        print("error occ")
        db.rollback()
    
    db.close()


def test_db():

    db = MySQLdb.connect("localhost", "root", "szkbsgy2019", "stock_history_info", charset='utf8')
    try:
        print("hello")
    except:
        print("connect failed")
    cursor = db.cursor()

    #sql="""insert into stock_date_601857(code, name, today_start,today_peak,today_low, yesterday_end,trust_rate,
     #   exchange_rate, range_rate, deal_volume,date) values("%s","%s",%f,%f,%f,%f,%f,%f,%f,%f,"%s")"""%(code,name, today_start,today_peak,today_low,\
      #      yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)

    #sql="""insert into stock_history_sh601857(code, name, today_start, today_close, today_max, today_min, today_deal_volume, today_deal_CNY, average_price, date) values("sh601857","", "6.670", "6.670", "6.690", "6.640","48044859","320410969","6.66","20190712")"""
    sql="""insert into stock_history_sh601857(code, date) values("sh601857", "20190712")"""
    print(sql)
    try:
        cursor.execute(sql)
        db.commit()
    except:
        print("error occ")
        db.rollback()
    
    db.close()

def test():

    db = MySQLdb.connect("localhost", "root", "szkbsgy2019", "stock_info", charset='utf8')

    cursor = db.cursor()

    #sql="""insert into stock_date_601857(code, name, today_start,today_peak,today_low, yesterday_end,trust_rate,
     #   exchange_rate, range_rate, deal_volume,date) values("%s","%s",%f,%f,%f,%f,%f,%f,%f,%f,"%s")"""%(code,name, today_start,today_peak,today_low,\
      #      yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)

    sql="""insert into stock_history_sh601857(code, date) values("sh601857", "20190712")"""
    print(sql)

    try:
        cursor.execute(sql)
        db.commit()
    except:
        print("error occ")
        db.rollback()

    db.close()

test()
