import requests
from bs4 import BeautifulSoup
import traceback
import re
import sys
from db_test import stock_store_into_db


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


def store_into_db():
    infoDict = {}



def get_stock_list(slist):
    slist.append('sh600175')
    slist.append('sh601857')

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

def main():
    stock_list_url = 'http://quote.eastmoney.com/stocklist.html'
    stock_info_url = 'http://gupiao.baidu.com/stock/'
    output_file = './StockInfo.txt'
    slist = []
    #getStockList(slist, stock_list_url)
    #get_stock_list(slist)
    #print(slist)
    #getStockInfo(slist, stock_info_url, output_file)
    stock_store_into_db(code, name, today_start, today_peak, today_low, yesterday_end, trust_rate, exchange_rate, range_rate, deal_volume, date)

main()
