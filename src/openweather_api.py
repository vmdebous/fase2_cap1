
import os
from dotenv import load_dotenv
import requests
import json

load_dotenv()


cidade = "Sao Paulo,BR"
open_weather_api_key = os.getenv("OPEN_WEATHER_API_KEY")

url = f"https://api.openweathermap.org/data/2.5/weather?q=Sao Paulo,BR&appid={open_weather_api_key}&units=metric"
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    
    # salvar JSON em arquivo
    with open("clima.txt", "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)
    
    print("Arquivo salvo com sucesso!")
else:
    print("Erro:", response.text)
    print("Status:", response.status_code)
