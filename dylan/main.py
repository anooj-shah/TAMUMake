import wolframalpha
import json
from PIL import Image
import requests
from io import BytesIO

secret = ''
client = wolframalpha.Client(secret)

query = ''
while True:
    query = input()
    res = client.query(query)

    count = 0
    neededPod = 0
    for pod in res.pods:
        if (count == 0):
            neededPod = pod
            break
    resString = json.dumps(neededPod)
    resJson = json.loads(resString)

    imgSrc = resJson['subpod']['img']['@src']
    print()
    print(resJson['subpod']['img']['@alt'])
    print(imgSrc)
    imgResponse = requests.get(imgSrc)
    img = Image.open(BytesIO(imgResponse.content))
    print(img)
    img.show()