from bs4 import BeautifulSoup
import re
import requests
import json
import sys

if len(sys.argv) != 2:
    print("Usage: python script utilizes 1 argument (arg1)")
    sys.exit(1)


def web_scrape(url):
    page = requests.get(url)
    soup = BeautifulSoup(page.text, features='html.parser')
    
    #Get title and date and combine to report number
    title = soup.find('title').text.strip().split('|')[0].strip()
    nums = re.findall(r'\d+', title)
    if len(nums) < 4:
        nums.append('0')
    for i in range(len(nums)):
        if len(nums[i]) < 2:
            nums[i] = '0'+nums[i]
    num_str = "".join([str(i) for i in nums])

    #Get list of decks
    li = soup.find_all('li')
    all_li = [i.text.strip() for i in li]
    list_decks = []
    pattern = r'^\d+ [^\s].*$'
    for i in all_li:
        if re.match(pattern, i):
            list_decks.append(i)
    pattern = r'^[^()]*'
    for i in range(len(list_decks)):
        temp = re.search(pattern, list_decks[i])
        if temp:
            list_decks[i] = temp.group().strip()
    for i in range(len(list_decks)):
        list_decks[i] = list_decks[i].split(" ", 1)
        list_decks[i][0] = int(list_decks[i][0])
    ocg = {}
    for i in list_decks:
        ocg[i[1]] = i[0]


    #Return in JSON format
    model_data = { "title": title,
                "data": ocg,
                "report_num": num_str
                }
    return json.dumps(model_data)

result = web_scrape(sys.argv[1])
print(result)

