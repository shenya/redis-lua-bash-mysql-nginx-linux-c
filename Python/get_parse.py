import requests
from bs4 import BeautifulSoup
import traceback
import re
import sys
import time
from db_test import stock_store_into_db
import random

def get_rand_number(a, b):

    c = random.randint(a,b) + random.randint(a,b) + 3;
    print ("random() : ", c)

    return c

def getHTMLText(url):
    try:
        r = requests.get(url)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return ""

def getStockList(lst, stockURL):
    html = getHTMLText(stockURL)
    soup = BeautifulSoup(html, 'html.parser')
    a = soup.find_all('a')
    for i in a:
        try:
            href = i.attrs['href']
            lst.append(re.findall(r'[s][hz]\d{6}', href)[0])
            print(lst)
        except:
            continue
def getStockInfo(lst, stockURL, fpath):
    for stock in lst:
        url = stockURL + stock + ".html"
        html = getHTMLText(url)
        try:
            if html == "":
                continue
            infoDict = {}
            soup = BeautifulSoup(html, 'html.parser')
            stockInfo = soup.find('div', attrs={'class': 'stock-bets'})

            name = stockInfo.find_all(attrs={'class': 'bets-name'})[0]
            infoDict.update({'name': name.text.split()[0]})
            keyList = stockInfo.find_all('dt')
            valueList = stockInfo.find_all('dd')
            for i in range(len(keyList)):
                key = keyList[i].text
                val = valueList[i].text
                infoDict[key] = val
                with open(fpath, 'w', encoding='utf-8') as f:
                    f.write(str(infoDict) + '\n')
        except:
            traceback.print_exc()
            continue

def getStockInfo_to_db(lst, stockURL, date):
    for stock in lst:
        sleep_time = get_rand_number(1, 10)
        print(sleep_time)
        time.sleep(sleep_time)
        url = stockURL + stock + ".html"
        html = getHTMLText(url)
        try:
            if html == "":
                continue
            infoDict = {}
            soup = BeautifulSoup(html, 'html.parser')
            stockInfo = soup.find('div', attrs={'class': 'stock-bets'})

            name = stockInfo.find_all(attrs={'class': 'bets-name'})[0]
            close_price_id = stockInfo.find_all(attrs={'class': '_close'})[0]
            close_price = close_price_id.text.split()[0]
            #print(close_price)

            try:
                close_variable_id = stockInfo.find_all(attrs={'class': 'price s-down'})[0]
            except:
                close_variable_id = stockInfo.find_all(attrs={'class': 'price s-up'})[0]
            #print(close_variable_id)
            close_variable_id_array = close_variable_id.find_all('span')
            today_change = close_variable_id_array[0].text.split()[0]
            today_change_rate = close_variable_id_array[1].text.split()[0]
            #print(today_change)
            #print(today_change_rate)
            #infoDict.update({'name': name.text.split()[0]})
            table="stock_date_"+stock
            code=stock
            name=name.text.split()[0];
            keyList = stockInfo.find_all('dt')
            valueList = stockInfo.find_all('dd')
            today_start=valueList[0].text;
            today_peak=valueList[2].text
            today_low=valueList[13].text
            yesterday_end=valueList[11].text
            trust_rate=valueList[6].text
            exchange_rate=valueList[12].text
            range_rate=valueList[16].text
            deal_volume=valueList[1].text

            #print(date);
            stock_store_into_db(table, code, name, close_price, today_change, today_change_rate, today_start, today_peak, today_low, yesterday_end, trust_rate, exchange_rate, range_rate, deal_volume, date)
#            for i in range(len(keyList)):
#                key = keyList[i].text
#                val = valueList[i].text
#                print (i, key, val)
        except:
            traceback.print_exc()
            continue




def get_stock_list(slist):
    slist.append('sh600175')
    slist.append('sh601857')
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

code=601857
name='zgsy'
today_start=6.87
today_peak=6.7
today_low=7.7
yesterday_end=8.7
trust_rate=9.7
exchange_rate=10.1
range_rate=22.3
deal_volume=33.3
date='20190706'

running_log="./stock_running.txt"

def main():
    stock_list_url = 'http://quote.eastmoney.com/stocklist.html'
    stock_info_url = 'http://gupiao.baidu.com/stock/'
    output_file = './StockInfo.txt'
    slist = []
    running_result=""
    date=time.strftime("%Y%m%d", time.localtime())
    print(date)
    today_workdate=int(time.strftime("%w"))
    print(today_workdate)
    if today_workdate == 6 or today_workdate == 0:
        print ("this is non-work date")
        running_result="this is non-work date"
    else:
        get_stock_list(slist)
        print(slist)
        getStockInfo_to_db(slist, stock_info_url, date)
        running_result="this is work date"

    with open(running_log, 'a', encoding='utf-8') as f:
        f.write(date + running_result + '\n')
        f.close()
main()
