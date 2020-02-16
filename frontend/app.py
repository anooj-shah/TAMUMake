import os
from flask import Flask, render_template, request
import requests
import matplotlib.pyplot as plt
import json
from PIL import Image
from io import BytesIO
import serial



app = Flask(__name__)
app.config['ENV'] = 'development'
app.config['DEBUG'] = True
app.config['TESTING'] = True

@app.route('/')
def hello_world():
    return render_template('index.html')

@app.route('/create')
def create():
    return render_template('create.html')

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/message-original')
def original():
    return render_template('message-original.html')

@app.route('/message-love')
def love():
    return render_template('message-love.html')

@app.route('/message-thankyou')
def thanks():
    return render_template('message-thankyou.html')

@app.route('/message-holiday')
def holiday():
    return render_template('message-holiday.html')

@app.route('/message-happybirthday')
def happyBirthday():
    return render_template('message-happybirthday.html')

@app.route('/message-comedic')
def comedic():
    return render_template('message-comedic.html')

@app.route('/thank-you', methods=['POST'])
def getValues():
    # ser = serial.Serial('/dev/ttyUSB0')
    # print(ser.name)
    yourName = request.form['yourName']
    recipientName = request.form['recipientName']
    address = request.form['address']
    cardDesc = request.form['custId']
    message = request.form['message']
    print(cardDesc)
    # messageToSend = recipientName + '-' + message + '-' + cardDesc + '-' + yourName
    # messageToSend = messageToSend.encode('utf-8')
    # ser.write(messageToSend)
    # ser.close()
    return render_template('thankyou.html')

@app.route('/message-original', methods=['POST'])
def getVision():
    # ser = serial.Serial('/dev/ttyUSB0')
    # print(ser.name)

    yourName = request.form['yourName']
    recipientName = request.form['recipientName']
    address = request.form['address']
    image_url = request.form['imageURL']
    message = request.form['message']
    print(yourName, message)
    if 'COMPUTER_VISION_SUBSCRIPTION_KEY' in os.environ:
        subscription_key = os.environ['COMPUTER_VISION_SUBSCRIPTION_KEY']
    else:
        print("\nSet the COMPUTER_VISION_SUBSCRIPTION_KEY environment variable.\n**Restart your shell or IDE for changes to take effect.**")
        sys.exit()

    if 'COMPUTER_VISION_ENDPOINT' in os.environ:
        endpoint = os.environ['COMPUTER_VISION_ENDPOINT']

    analyze_url = endpoint + "vision/v2.1/analyze"
    # USE THIS::: https://images-na.ssl-images-amazon.com/images/I/61wtSL1Yd6L._AC_SX425_.jpg
    headers = {'Ocp-Apim-Subscription-Key': subscription_key}
    params = {'visualFeatures': 'Categories,Description,Color'}
    data = {'url': image_url}
    response = requests.post(analyze_url, headers=headers,
                             params=params, json=data)
    response.raise_for_status()
    analysis = response.json()
    print(json.dumps(response.json()))
    image_caption = analysis["description"]["captions"][0]["text"].capitalize()

    print(image_caption)
    # messageToSend = recipientName + '-' + message + '-' + image_caption + '-' + yourName
    # messageToSend = messageToSend.encode('utf-8')
    # ser.write(messageToSend)
    # ser.close()
    return render_template('message-original.html')
