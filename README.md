# How to run demo server
This server is only for demo and visulize the data flow in the system (Last update 16/6/2024)

## Server location
**Environment**: Visual Studio Code
**Repository**: /home/qtiot/.vscode-server/flask/flaskapp

## Rebuild server if any new update in source code
```bash
docker compose up --build
```

## Start server
```bash
docker compose up
```

## Foward port 5000 and open interface
```bash
http://127.0.0.1:5000/
```

## Fetch API at /decrypt
```bash
http://127.0.0.1:5000/api/decrypt
```

## Retrive from database at /retrive
```bash
http://127.0.0.1:5000/api/retrieve
```

## Run the request.py for demo in sublime text
```bash
cd..
D:
cd TAI LIEU HOC TAP\DESLAB\Internet of Medical Things\API Server PYTHON
```
