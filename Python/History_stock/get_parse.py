import requests
from bs4 import BeautifulSoup
import traceback
import re
import sys
import time
import random
from db_test import stock_history_store_into_db




running_log="./stock_running.txt"


def getHTMLText(url):
    try:
        r = requests.get(url)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return ""


def parse_day_stock_info(today_stock_info):
    #html_text="""
    #<tr>
    #<td><div align="center">
    #<a href="http://vip.stock.finance.sina.com.cn/quotes_service/view/vMS_tradehistory.php?symbol=sh601857&amp;date=2019-07-12" target="_blank">
    #2019-07-12 </a>
    #</div></td>
    #<td><div align="center">6.670</div></td>
    #<td><div align="center">6.690</div></td>
    #<td><div align="center">6.670</div></td>
    #<td class="tdr"><div align="center">6.640</div></td>
    #<td class="tdr"><div align="center">48044859</div></td>
    #<td class="tdr"><div align="center">320410969</div></td>
    #</tr>
    #"""

    #soup = BeautifulSoup(today_stock_info, 'html.parser')
    soup=today_stock_info
    #print(soup)

    item_list = soup.find_all('td')
    #print(len(item_list))
    #print(item_list[0])
    date=item_list[0].find_all('div')[0].text.split()[0].replace('-','');
    #print("date:"+date);
    #print(item_list[1])
    today_start=item_list[1].find_all('div')[0].text.split()[0]
    #print("today start:"+today_start)
    #print(item_list[2])
    today_max=item_list[2].find_all('div')[0].text.split()[0]
    #print("today_max:"+today_max)
    #print(item_list[3])
    today_close=item_list[3].find_all('div')[0].text.split()[0]
    #print("today close:"+today_close)
    #print(item_list[4])
    today_min=item_list[4].find_all('div')[0].text.split()[0]
    #print("today_min:"+today_min)
    #print(item_list[5])
    today_deal_volume=item_list[5].find_all('div')[0].text.split()[0]
    #print("today_deal_volume:" +today_deal_volume)
    #print(item_list[6])
    today_deal_CNY=item_list[6].find_all('div')[0].text.split()[0]
    #print("today_deal_CNY:"+today_deal_CNY)

    average_price = float(today_deal_CNY) / int(today_deal_volume)
    #print("average_price:%f" %average_price)

    return date,today_start,today_max,today_close,today_min,today_deal_volume,today_deal_CNY,average_price

def get_stock_and_store_into_db(stock_code, year, season, today_stock_info):
    date,today_start,today_max,today_close,today_min,today_deal_volume,today_deal_CNY,average_price=parse_day_stock_info(today_stock_info)
    #print("store into db: date:" +date)
    #to db
    #stock_history_store_into_db(table, code, name, today_start, today_max, today_close, today_min, today_deal_volume, today_deal_CNY date)
    table = "stock_history_"+str(stock_code)
    #print("table:"+table)
    stock_history_store_into_db(table, stock_code, '', today_start, today_max, today_close, today_min, today_deal_volume, today_deal_CNY, average_price, date)

def stock_query(stock_code, year, season, site_url):
    realcode=stock_code[2:]
    #stock_url = 'http://money.finance.sina.com.cn/corp/go.php/vMS_MarketHistory/stockid/601857.phtml?year=2019&jidu=3'
    stock_url=site_url+realcode+".phtml?year="+year+"&jidu="+season
    #print("stock_url:"+stock_url)

    html = getHTMLText(stock_url)
    try:
        if html == "":
            return
        soup = BeautifulSoup(html, 'html.parser')
        stockInfo = soup.find_all('table', id="FundHoldSharesTable")

        #print(stockInfo[0])

        #print("***********************************");
        day_stock_block = stockInfo[0].find_all('tr');

        #print(day_stock_block[0])
        #print(day_stock_block[1])
        #print(day_stock_block[2])
        #print(day_stock_block[3])
        day_stock_block.remove(day_stock_block[0])
        day_stock_block.remove(day_stock_block[0])
        day_stock_block.reverse()
        iter_stock=iter(day_stock_block)
        #next(iter_stock)
        #next(iter_stock)
        for stock_block in iter_stock:
            #print("#################################")
            #print(stock_block)
            get_stock_and_store_into_db(stock_code, year, season, stock_block)

    except:
        traceback.print_exc()



def get_stock_list(slist):
    slist.append('sh601857')
    slist.append('sh600175')
    slist.append('sh601169')
    slist.append('sh600266')
    slist.append('sh600050')
    slist.append('sh600008')
    slist.append('sh601992')
    slist.append('sh600285')
    slist.append('sh600284')
    slist.append('sh600999')
    slist.append('sh600106')
    slist.append('sz002157')
    slist.append('sz000725')

def main():
    #stock_info_url = 'http://money.finance.sina.com.cn/corp/go.php/vMS_MarketHistory/stockid/601857.phtml?year=2019&jidu=3'
    query_url = 'http://money.finance.sina.com.cn/corp/go.php/vMS_MarketHistory/stockid/'
    #stock_query(stock_info_url);

    #parse_day_stock_info();
    years=['2019']
    seasons=['1','2','3']
    slist = []
    get_stock_list(slist)

    for stock in slist:
        print("stock:"+stock)
        for year in years:
            print("year:"+year)
            for season in seasons:
                print("season:"+season)
                stock_query(stock, year, season, query_url)
main()
