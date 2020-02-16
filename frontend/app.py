from flask import Flask, render_template
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

@app.route('/temp2')
def temp2():
    return render_template('contact.html')