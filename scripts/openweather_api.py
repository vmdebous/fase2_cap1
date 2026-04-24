
import os
from dotenv import load_dotenv
import requests
import json
import pandas as pd
load_dotenv()
import datetime as dt
import argparse

parser = argparse.ArgumentParser(description="Integração R <-> Python")

parser.add_argument(
    "--mode",
    choices=["to_R", "from_R"],
    required=True
)

args = parser.parse_args()

if args.mode == "to_R":
    cidade = "Sao Paulo,BR"
    open_weather_api_key = os.getenv("OPEN_WEATHER_API_KEY")

    url = f"https://api.openweathermap.org/data/2.5/weather?q=Sao Paulo,BR&appid={open_weather_api_key}&units=metric"

    response = requests.get(url)


    if response.status_code == 200:
        data = response.json()
        
        with open("diagram.json", "r", encoding="utf-8") as f:
            diagram = json.loads(f.read())
            diagram['files']['clima.txt'] = json.loads(diagram['files']['clima.txt'])
            print(diagram)
            if dt.date.today().strftime("%Y-%m-%d") not in diagram['files']['clima.txt'].keys():
                diagram['files']['clima.txt'][dt.date.today().strftime("%Y-%m-%d")] = {"manha":None,"tarde":None,"noite":None}
            if dt.datetime.now().hour < 12:
                diagram['files']['clima.txt'][dt.date.today().strftime("%Y-%m-%d")]["manha"] = (data['weather'][0]['id']//100)*100 if (data['weather'][0]['id']//100)*100 != 700 else 800
            elif dt.datetime.now().hour < 18:
                diagram['files']['clima.txt'][dt.date.today().strftime("%Y-%m-%d")]["tarde"] = (data['weather'][0]['id']//100)*100 if (data['weather'][0]['id']//100)*100 != 700 else 800
            else:
                diagram['files']['clima.txt'][dt.date.today().strftime("%Y-%m-%d")]["noite"] = (data['weather'][0]['id']//100)*100 if (data['weather'][0]['id']//100)*100 != 700 else 800
        
        to_r = pd.DataFrame(diagram['files']['clima.txt'].values(),\
                        index=diagram['files']['clima.txt'].keys(),\
                        columns=['manha','tarde','noite']).reset_index().rename(columns={"index":"data"})
        to_r['manha'] = to_r['manha'].apply(lambda x: float(x) if str(x) != 'nan'  else None)
        to_r['tarde'] = to_r['tarde'].apply(lambda x: float(x) if str(x) != 'nan' else None)
        to_r['noite'] = to_r['noite'].apply(lambda x: float(x) if str(x) != 'nan' else None)
        to_r.to_excel("to_R.xlsx")
        diagram['files']['clima.txt'] = json.dumps(diagram['files']['clima.txt'])
        print(json.dumps(diagram))
        # with open("diagram.json", "w", encoding="utf-8") as f:
        #     f.write(json.dumps(diagram, indent=4))
        
        print("Arquivo salvo com sucesso!")
    else:
        print("Erro:", response.text)
        print("Status:", response.status_code)
elif args.mode == "from_R":
    df = pd.read_excel("from_R.xlsx")
    print(df)
    with open("diagram.json", "r+", encoding="utf-8") as f:
        diagram = json.loads(f.read())

        diagram['files']['predicao.txt'] = "true" if not df[df['prob'].isin([500,200])].empty else "false"


            