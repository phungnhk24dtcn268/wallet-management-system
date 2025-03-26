FROM ubuntu:22.04

RUN apt-get update && apt-get install -y g++ make

COPY . /app
WORKDIR /app

RUN g++ -o wallet main.cpp src/*.cpp -Iinclude

CMD ["./wallet"]
