import requests
import json

url = "http://127.0.0.1:5000/update-bin-level"
data = {"fill_level": 85}

# Send POST request
response = requests.post(url, json=data)

# Print the response
print(response.json())
