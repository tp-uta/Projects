import pandas as pd
import sys
import json

if len(sys.argv) != 3:
    print("Usage: python script utilizes 2 arguments (arg1 and arg2)")
    sys.exit(1)

arg1 = sys.argv[1]
arg2 = sys.argv[2]

def convert_dict(arg):
    temp = json.loads(arg)
    total = 0
    for i in temp:
        total += temp[i]
    return temp, total

def data_analysis(arg1, arg2):
    #arg will be a json string format, convert to pandas df
    ocg1, total1 = convert_dict(arg1)
    ocg2, total2 = convert_dict(arg2)

    report = {}
    for deck in ocg1:
        if deck in ocg2:
            report[deck] = [ocg1[deck], ocg2[deck]]
            del ocg2[deck]
        else:
            report[deck] = [ocg1[deck], 0]
    for deck in ocg2:
        report[deck] = [0, ocg2[deck]]
    
    df = pd.DataFrame({'Name': [deck for deck in report],
                       'Old': [report[deck][0] for deck in report],
                       'New': [report[deck][1] for deck in report]})
    df["Percent_Change"] = (df.iloc[:,2]/total2 - df.iloc[:,1]/total1)*100
    df["Depreciated"] = [True if x == 0 else False for x in df.iloc[:,2]]
    df["Appreciated"] = [True if x == 0 else False for x in df.iloc[:,1]]
    df = df.sort_values(by=['New'], ascending=False)
    df = df.reset_index(drop=True)
    return df.to_json(orient='records')

result = data_analysis(arg1, arg2)
print(result)