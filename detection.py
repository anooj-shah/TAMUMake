import io
import os
import requests
# Imports the Google Cloud client library
from google.cloud import vision
from google.cloud.vision import types


def detect():
    # Instantiates a client
    client = vision.ImageAnnotatorClient()

    # The name of the image file to annotate
    file_name = os.path.abspath('./image2.png')

    # Loads the image into memory
    with io.open(file_name, 'rb') as image_file:
        content = image_file.read()

    image = types.Image(content=content)

    # Performs label detection on the image file
    response = client.label_detection(image=image)
    print(response)
    labels = response.label_annotations

    for label in labels:
        print(label.description)

def detect2():
    url = 'https://westus.api.cognitive.microsoft.com/vision/v2.0/describe?maxCandidates=1'
    myobj = {'somekey': 'somevalue'}
    x = requests.post(url, data = myobj)
    print(x.text)
