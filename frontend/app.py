import os
from flask import Flask, render_template
import requests
import matplotlib.pyplot as plt
import json
from PIL import Image
from io import BytesIO


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

@app.route('/temp')
def temp():
    return render_template('work-single.html')

@app.route('/temp2', methods=['POST'])
def temp2():
    name = request.form['name']
    image_url = request.form['image_url']
    id = request.args.get('id')

    if 'COMPUTER_VISION_SUBSCRIPTION_KEY' in os.environ:
        subscription_key = os.environ['COMPUTER_VISION_SUBSCRIPTION_KEY']
    else:
        print("\nSet the COMPUTER_VISION_SUBSCRIPTION_KEY environment variable.\n**Restart your shell or IDE for changes to take effect.**")
        sys.exit()

    if 'COMPUTER_VISION_ENDPOINT' in os.environ:
        endpoint = os.environ['COMPUTER_VISION_ENDPOINT']

    analyze_url = endpoint + "vision/v2.1/analyze"

    # image_url = "https://image.shutterstock.com/image-photo/close-photo-cheerful-excited-glad-260nw-789414166.jpg"

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
    return image_caption
