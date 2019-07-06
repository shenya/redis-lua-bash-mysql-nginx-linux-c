
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


def stock_store_into_db(code, name, today_start, today_peak, today_low, yesterday_end, trust_rate, exchange_rate, range_rate, deal_volume, date):

    db = MySQLdb.connect("localhost", "root", "szkbsgy2019", "stock_info", charset='utf8')

    cursor = db.cursor()

    sql="""insert into stock_date_601857(code, name, today_start,today_peak,today_low, yesterday_end,trust_rate,
        exchange_rate, range_rate, deal_volume,date) values(%d,"%s",%f,%f,%f,%f,%f,%f,%f,%f,"%s")"""%(code,name, today_start,today_peak,today_low,\
            yesterday_end,trust_rate, exchange_rate, range_rate, deal_volume, date)

    print(sql)

    try:
        cursor.execute(sql)
        db.commit()
    except:
        print("error occ")
        db.rollback()

    db.close()

#stock_store_into_db(code, name, today_start, today_peak, today_low, yesterday_end, trust_rate, exchange_rate, range_rate, deal_volume, date)
