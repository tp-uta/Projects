from dotenv import load_dotenv
from bs4 import BeautifulSoup
import re
import os
import requests
import pandas as pd
import json

load_dotenv()

urls = json.loads(os.environ['website'])

url = urls[0]

page = requests.get(url)
soup = BeautifulSoup(page.text, features='lxml')

#Get title and date
title = soup.find('title')
date = title.text.strip().split(' ')[1].split('.')
date[0], date[1] = date[1], date[0]
title = title.text.split('|')[0].strip()

#Get list of decks
temp_list = soup.find_all("li")
arr_list = [i.text.strip() for i in temp_list]
list_decks = []
pattern = r'^\d+ [^\s].*$'
for i in arr_list:
    if re.match(pattern, i):
        list_decks.append(i)

#Formatting the decks
pattern = pattern = r'^[^()]*'
for i in range(len(list_decks)):
    temp = re.search(pattern, list_decks[i])
    if temp:
        list_decks[i] = temp.group().strip()
for i in range(len(list_decks)):
    list_decks[i] = list_decks[i].split(" ", 1)
    list_decks[i][0] = int(list_decks[i][0])

dict_decks = {}
total = 0
for i in list_decks:
    dict_decks[i[1]] = i[0]
    total += i[0]

#Get 2 to run:
report = {}
for deck in list1:
    if deck in list2:
        report[deck] = [list1[deck], list2[deck]]
        del list2[deck]
    else:
        report[deck] = [list1[deck], 0]
for deck in list2:
    report[deck] = [0, list2[deck]]

#Dataframe:
df = pd.DataFrame({'Name': [deck for deck in report],
                   f'{title1}': [report[deck][0] for deck in report],
                   f'{title2}': [report[deck][1] for deck in report]})
df["Percent Change"] = (df.iloc[:,2]/total2 - df.iloc[:,1]/total1)*100
df["Depreciated"] = [True if x == 0 else False for x in df.iloc[:,2]]
df["Appreciated"] = [True if x == 0 else False for x in df.iloc[:,1]]
print(df)